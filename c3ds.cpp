/***************************************************************************/
/*                                                                         */
/*                            www.xbdev.net                                */
/*                         bkenwright@xbdev.net                            */
/*                                3ds.cpp                                  */
/*                                                                         */
/***************************************************************************/

#include "c3ds.h"
#include <QImage>

C3DS::C3DS(char *szFileName)
{
    m_iNumMeshs     = 0;
    m_iNumMaterials = 0;
    this->scale.x = 1;
    this->scale.y = 1;
    this->scale.z = 1;
    this->load(szFileName);
}

C3DS::~C3DS(void){}


/***************************************************************************/
/*                                                                         */
/* Some user functions to make the reading of the .3ds file easier         */
/*                                                                         */
/***************************************************************************/
/* Helper Functions that make our parsing of the chunks easier             */
/***************************************************************************/


void C3DS::ReadChunk(stChunk *pChunk)
{
    unsigned short ID             = 0;
    unsigned int bytesRead        = 0;
    unsigned int bChunkLength     = 0;

    bytesRead = (unsigned int)fread(&ID, 1, 2, m_fp);

    bytesRead += (unsigned int)fread(&bChunkLength, 1, 4, m_fp);

    pChunk->ID          = ID;
    pChunk->length      = bChunkLength;
    pChunk->bytesRead = bytesRead;

}

void C3DS::SkipChunk(stChunk *pChunk)
{
    int buffer[50000] = {0};

    fread(buffer, 1, pChunk->length - pChunk->bytesRead, m_fp);
}

/***************************************************************************/
/*                                                                         */
/* Helper Fuction, simply reads in the string from the file pointer, until */
/* a null is reached, then returns how many bytes was read.                */
/*                                                                         */
/***************************************************************************/
int C3DS::GetString(char* pBuffer)
{
    int index = 0;

    char buffer[100] = {0};

    fread(buffer, 1, 1, m_fp);

    while( *(buffer + index++) != 0)
    {
        fread(buffer + index, 1, 1, m_fp);
    }

    strcpy( pBuffer, buffer );

    return (int)(strlen(buffer) + 1);
}

/***************************************************************************/
/*                                                                         */
/* This little function reads the matrial data for our individual object,  */
/* So it determines which face references which material, in our material  */
/* list.                                                                   */
/*                                                                         */
/***************************************************************************/
void C3DS::ReadMeshMaterials(stChunk* Chunk)
{
    // Material Name Where Referencing
    char str[256];
    unsigned int characterlen = GetString(str);
    Chunk->bytesRead += characterlen;

    unsigned short iNumFaces = 0;
    Chunk->bytesRead += (unsigned int)fread(&iNumFaces, 1, 2, m_fp);

    unsigned short *FaceAssignedThisMaterial = new unsigned short[iNumFaces];
    Chunk->bytesRead += (unsigned int)fread(FaceAssignedThisMaterial, 1,
                                            iNumFaces*sizeof(unsigned short), m_fp);

    // Determine Which Material It Is In Our List
    int MaterialID = 0;
    for( int cc=0; cc<m_iNumMaterials; cc++)
    {
        if( strcmp( str, m_pMaterials[cc].szName ) == 0 )
            MaterialID = cc;
    }

    stMesh* pMesh = &(m_pMeshs[m_iNumMeshs - 1]);
    for(int i=0; i<iNumFaces; i++)
    {
        int iIndex = FaceAssignedThisMaterial[i];
        pMesh->pFaces[iIndex].materialID = MaterialID;
    }

    return;
}

/***************************************************************************/
/*                                                                         */
/* We get all the faces...e.g. Triangle Index's into our vertex array, so  */
/* we can actually put our shape together.                                 */
/*                                                                         */
/***************************************************************************/
void C3DS::ReadMeshFaces(stChunk* Chunk)
{
    unsigned int iNumberFaces = 0; //= Chunk->length - 6;
    Chunk->bytesRead += (unsigned int)fread(&iNumberFaces, 1, 2, m_fp);

    // Each face is 3 points A TRIANGLE!..WOW
    struct stXFace{ unsigned short vertIds[3], visibityflag; };
    stXFace *pFaces = new stXFace[iNumberFaces];

    Chunk->bytesRead += (unsigned int)fread(pFaces, 1, iNumberFaces*sizeof(stXFace), m_fp);

    stMesh* pMesh = &(m_pMeshs[m_iNumMeshs - 1]);
    pMesh->pFaces = new stFace[iNumberFaces];
    pMesh->iNumFaces = iNumberFaces;

    for( unsigned int i=0; i<iNumberFaces; i++ )
    {
        for(int y=0; y < 3; y++){
            pMesh->pFaces[i].vertIds[y] = pFaces[i].vertIds[y];
        }
    }

    delete[] pFaces;


    // Our face material information is a sub-chunk.
    ParseChunk(Chunk);
}

/***************************************************************************/
/*                                                                         */
/* You know all those x,y,z things...yup I mean vertices...well this reads */
/* them all in.                                                            */
/*                                                                         */
/***************************************************************************/
void C3DS::ReadMeshVertices(stChunk* Chunk)
{
    unsigned int iNumberVertices = 0;
    Chunk->bytesRead += (unsigned int)fread(&iNumberVertices, 1, 2, m_fp);

    // Allocate Memory and dump our vertices to the screen.
    stVert *pVerts = new stVert[iNumberVertices];
    stVert *pNormalVerts = new stVert[iNumberVertices];

    Chunk->bytesRead += (unsigned int)fread( (void*)pVerts, 1, iNumberVertices*sizeof(stVert), m_fp);

    stMesh* pMesh = &(m_pMeshs[m_iNumMeshs - 1]);
    pMesh->pVerts = pVerts;
    pMesh->pNormalVerts = pNormalVerts;
    pMesh->iNumVerts = iNumberVertices;

    SkipChunk(Chunk);
}

/***************************************************************************/
/*                                                                         */
/* Well if we have a texture, e.g. coolimage.bmp, then we need to load in  */
/* our texture coordinates...tu and tv.                                    */
/*                                                                         */
/***************************************************************************/
void C3DS::ReadMeshTexCoords(stChunk* Chunk)
{
    unsigned short iNumberVertices = 0;
    Chunk->bytesRead += (unsigned int)fread(&iNumberVertices, 1, 2, m_fp);

    // Allocate Memory and dump our texture for each vertice to the screen.
    stTex *pTex = new stTex[iNumberVertices];

    Chunk->bytesRead += (unsigned int)fread( (void*)pTex, 1, iNumberVertices*sizeof(stTex), m_fp);

    stMesh* pMesh = &(m_pMeshs[m_iNumMeshs - 1]);
    pMesh->pTexs = pTex;

    SkipChunk(Chunk);
}


/***************************************************************************/
/*                                                                         */
/* Read in our objects name...as each object in our 3D world has a name,   */
/* for example Box1, HillMesh...whatever you called your object or object's*/
/* in 3d max before you saved it.                                          */
/*                                                                         */
/***************************************************************************/
void C3DS::GetMeshObjectName(stChunk* Chunk)
{
    // The strange thing is, the next few parts of this chunk represent
    // the name of the object.  Then we start chunking again.
    char str[256];
    unsigned int characterlen = GetString(str);
    Chunk->bytesRead += characterlen;

    stMesh* pMesh = &(m_pMeshs[m_iNumMeshs - 1]);
    strcpy( pMesh->szMeshName, str );

    ParseChunk(Chunk);
}

// Read in our texture's file name (e.g. coolpic.jpg)
void C3DS::GetTexFileName(stChunk* Chunk)
{
    char str[256];
    Chunk->bytesRead += GetString(str);

    stMaterial* pMat = &(m_pMaterials[m_iNumMaterials-1]);
    strcpy(pMat->szTextureFile, str);

    this->GenerateTexture(pMat);
}


void C3DS::GenerateTexture(stMaterial *m){
    QImage image;
    if(image.load(QString("./data/images/") + m->szTextureFile)){
        glGenTextures(1, &m->GLTextureId);
        glBindTexture(GL_TEXTURE_2D, m->GLTextureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT ) ;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT ) ;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
    }
}

// Read in our diffuse colour (rgb)
void C3DS::GetDiffuseColour(stChunk* Chunk)
{
    struct stRGB{ unsigned char r, g, b; };
    stRGB DiffColour;

    char ChunkHeader[6];
    Chunk->bytesRead += (unsigned int)fread(ChunkHeader, 1, 6, m_fp);

    Chunk->bytesRead += (unsigned int)fread(&DiffColour, 1, 3, m_fp);

    stMaterial* pM = &(m_pMaterials[m_iNumMaterials-1]);
    pM->Colour.r = DiffColour.r;
    pM->Colour.g = DiffColour.g;
    pM->Colour.b = DiffColour.b;

    SkipChunk(Chunk);
}

// Get the materials name, e.g. default-2- etc
void C3DS::GetMaterialName(stChunk* Chunk)
{
    char str[256];
    Chunk->bytesRead += GetString(str);

    stMaterial* pM = &(m_pMaterials[m_iNumMaterials-1]);
    strcpy(pM->szName, str);
}
/***************************************************************************/
/*                                                                         */
/* If theres a nested sub-chunk, and we know its ID, e.g 0xA000 etc, then  */
/* we can simply add its ID to the switch list, and add a calling sub      */
/* functino which will deal with it.  Else just skip over that Chunk...    */
/* and carry on parsing the rest of our file.                              */
/*                                                                         */
/***************************************************************************/

void C3DS::ParseChunk(stChunk* Chunk)
{
    while(Chunk->bytesRead < Chunk->length)
    {
        stChunk tempChunk = {0, 0, 0};
        ReadChunk(&tempChunk);

        switch( tempChunk.ID)
        {
        // HEADER OUR ENTRY POINT
        case EDIT3DS: //0x3D3D
            ParseChunk(&tempChunk);
            break;

            // MATERIALS
        case MATERIAL: //0xAFFF
            stMaterial newMaterial;
            m_pMaterials.push_back(newMaterial);
            m_iNumMaterials++;
            ParseChunk(&tempChunk);
            break;
        case MAT_NAME: //0xA000 - sz for hte material name "e.g. default 2"
            GetMaterialName(&tempChunk);
            break;
        case MAT_DIFFUSE:  // Diffuse Colour  //0xA020
            GetDiffuseColour(&tempChunk);
            break;
        case MAT_TEXMAP:  //0xA200 - if there's a texture wrapped to it where here
            ParseChunk(&tempChunk);
            break;
        case MAT_TEXFLNM: // 0xA300 -  get filename of the material
            GetTexFileName(&tempChunk);
            break;

            // OBJECT - MESH'S
        case NAMED_OBJECT: //0x4000
        {
            stMesh newMesh;
            m_pMeshs.push_back(newMesh);
            m_iNumMeshs++;
            GetMeshObjectName(&tempChunk);
        }
            break;
        case OBJ_MESH:     //0x4100
            ParseChunk(&tempChunk);
            break;
        case MESH_VERTICES: //0x4110
            ReadMeshVertices(&tempChunk);
            break;
        case MESH_FACES: //0x4120
            ReadMeshFaces(&tempChunk);
            break;
        case MESH_TEX_VERT: //0x4140
            ReadMeshTexCoords(&tempChunk);
            break;
        case MESH_MATER: //0x4130
            ReadMeshMaterials(&tempChunk);
            break;

        default:
            SkipChunk(&tempChunk);
        }

        Chunk->bytesRead += tempChunk.length;
    }
}

/***************************************************************************/
/*                                                                         */
/* Read in .3ds file.                                                      */
/*                                                                         */
/***************************************************************************/

bool C3DS::load(char* szFileName)
{
    m_fp = fopen(szFileName, "rb");

    stChunk Chunk = {0, 0, 0};
    ReadChunk(&Chunk);

    ParseChunk(&Chunk );

    fclose(m_fp);

    this->calculateNormal();
    this->compile();

    return true;
}

void C3DS::release()
{
    for(int i=0; i<m_iNumMeshs; i++)
    {
        delete[] m_pMeshs[i].pVerts;
        delete[] m_pMeshs[i].pFaces;
        delete[] m_pMeshs[i].pTexs;
    }
}

void C3DS::compile()
{
    this->glListId = glGenLists(1);
    unsigned int vertId;
    unsigned int lastTextureId = -1;
    glNewList(this->glListId, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    for(int i=0; i < m_iNumMeshs; i++)
    {
        stMesh *pMesh = &this->m_pMeshs[i];

        for(int y = 0; y < pMesh->iNumFaces; y++){
            stFace *pFace = &pMesh->pFaces[y];

            glColor3f(1.0f, 1.0f, 1.0f);
            if(lastTextureId != m_pMaterials[pFace->materialID].GLTextureId){
                glBindTexture(GL_TEXTURE_2D, m_pMaterials[pFace->materialID].GLTextureId);
                lastTextureId = m_pMaterials[pFace->materialID].GLTextureId;
            }

            for(int p = 0; p < 3; p++){
                vertId = pFace->vertIds[p];

                glTexCoord2f(pMesh->pTexs[vertId].tu, pMesh->pTexs[vertId].tv);
                glNormal3f(pMesh->pNormalVerts[vertId].x, pMesh->pNormalVerts[vertId].y, pMesh->pNormalVerts[vertId].z);
                glVertex3f(pMesh->pVerts[vertId].x*this->scale.x, pMesh->pVerts[vertId].y*this->scale.y, pMesh->pVerts[vertId].z*this->scale.z);

                min.x = Min(min.x, pMesh->pVerts[vertId].x*this->scale.x);
                max.x = Max(max.x, pMesh->pVerts[vertId].x*this->scale.x);
                min.y = Min(min.y, pMesh->pVerts[vertId].y*this->scale.y);
                max.y = Max(max.y, pMesh->pVerts[vertId].y*this->scale.y);
                min.z = Min(min.z, pMesh->pVerts[vertId].z*this->scale.z);
                max.z = Max(max.z, pMesh->pVerts[vertId].z*this->scale.z);
            }

        }
    }
    glEnd();
    glEndList();
}

void C3DS::render()
{
    glBindTexture(GL_TEXTURE_2D, m_pMaterials[0].GLTextureId);
    glCallList(this->glListId);
}

Point3f C3DS::getMinPoint(){
    return this->min;
}

Point3f C3DS::getMaxPoint(){
    return this->max;
}

void C3DS::calculateNormal(){
    for(int i=0; i < m_iNumMeshs; i++){
        stMesh *pMesh = &this->m_pMeshs[i];

        for(int y = 0; y < pMesh->iNumFaces; y++){
            stFace *pFace = &pMesh->pFaces[y];
            stVert tmp;
            for(int i=0; i < 3; i++){
                stVert *cur = &pMesh->pVerts[pFace->vertIds[i]];
                stVert *next = &pMesh->pVerts[pFace->vertIds[(i+1)%3]];

                tmp.x += ((cur->y - next->y) * (cur->z + next->z));
                tmp.y += ((cur->z - next->z) * (cur->x + next->x));
                tmp.z += ((cur->x - next->x) * (cur->y + next->y));
            }

            tmp.normalize();

            for(int i=0; i < 3; i++){
                stVert *vert = &pMesh->pNormalVerts[i];
                vert->x += tmp.x;
                vert->y += tmp.y;
                vert->z += tmp.z;
            }
        }
    }
}
