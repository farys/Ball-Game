#ifndef BOXMODEL_H
#define BOXMODEL_H
#include "c3ds.h"

class TileModel: protected C3DS {
public:
    TileModel(){
        stMesh *mesh = new stMesh;

        /* vertexs */
        mesh->pVerts = new stVert[24];
        mesh->pNormalVerts = new stVert[24];
        mesh->pTexs = new stTex[24];
        mesh->iNumVerts = 24;

        // dolna sciana
        mesh->pTexs[0].set(1.0f, 0.0f); mesh->pVerts[0].set(-1.0f, -1.0f,  1.0f);
        mesh->pTexs[1].set(0.0f, 0.0f); mesh->pVerts[1].set( 1.0f, -1.0f,  1.0f);
        mesh->pTexs[2].set(0.0f, 1.0f); mesh->pVerts[2].set( 1.0f, -1.0f, -1.0f);
        mesh->pTexs[3].set(1.0f, 1.0f); mesh->pVerts[3].set(-1.0f, -1.0f, -1.0f);
        // przednia sciana
        mesh->pTexs[7].set(0.0f, 0.0f); mesh->pVerts[7].set(-1.0f, -1.0f,  1.0f);
        mesh->pTexs[6].set(1.0f, 0.0f); mesh->pVerts[6].set( 1.0f, -1.0f,  1.0f);
        mesh->pTexs[5].set(1.0f, 1.0f); mesh->pVerts[5].set( 1.0f,  1.0f,  1.0f);
        mesh->pTexs[4].set(0.0f, 1.0f); mesh->pVerts[4].set(-1.0f,  1.0f,  1.0f);
        // tylnia sciana
        mesh->pTexs[11].set(1.0f, 0.0f); mesh->pVerts[11].set(-1.0f, -1.0f, -1.0f);
        mesh->pTexs[10].set(1.0f, 1.0f); mesh->pVerts[10].set(-1.0f,  1.0f, -1.0f);
        mesh->pTexs[9].set(0.0f, 1.0f); mesh->pVerts[9].set( 1.0f,  1.0f, -1.0f);
        mesh->pTexs[8].set(0.0f, 0.0f); mesh->pVerts[8].set( 1.0f, -1.0f, -1.0f);
        // prawa sciana
        mesh->pTexs[15].set(1.0f, 0.0f); mesh->pVerts[15].set( 1.0f, -1.0f, -1.0f);
        mesh->pTexs[14].set(1.0f, 1.0f); mesh->pVerts[14].set( 1.0f,  1.0f, -1.0f);
        mesh->pTexs[13].set(0.0f, 1.0f); mesh->pVerts[13].set( 1.0f,  1.0f,  1.0f);
        mesh->pTexs[12].set(0.0f, 0.0f); mesh->pVerts[12].set( 1.0f, -1.0f,  1.0f);
        // lewa sciana
        mesh->pTexs[19].set(0.0f, 0.0f); mesh->pVerts[19].set(-1.0f, -1.0f, -1.0f);
        mesh->pTexs[18].set(1.0f, 0.0f); mesh->pVerts[18].set(-1.0f, -1.0f,  1.0f);
        mesh->pTexs[17].set(1.0f, 1.0f); mesh->pVerts[17].set(-1.0f,  1.0f,  1.0f);
        mesh->pTexs[16].set(0.0f, 1.0f); mesh->pVerts[16].set(-1.0f,  1.0f, -1.0f);
        // gorna sciana
        mesh->pTexs[23].set(0.0f, 0.0f); mesh->pVerts[23].set(-1.0f, 1.0f,  1.0f);
        mesh->pTexs[22].set(1.0f, 0.0f); mesh->pVerts[22].set( 1.0f, 1.0f,  1.0f);
        mesh->pTexs[21].set(1.0f, 1.0f); mesh->pVerts[21].set( 1.0f, 1.0f, -1.0f);
        mesh->pTexs[20].set(0.0f, 1.0f); mesh->pVerts[20].set(-1.0f, 1.0f, -1.0f);

        /* faces */
        mesh->pFaces = new stFace[12];
        mesh->iNumFaces = 12;

        mesh->pFaces[0].set(0, 1, 2, 0);
        mesh->pFaces[1].set(0, 2, 3, 0);

        mesh->pFaces[2].set(4, 5, 6, 0);
        mesh->pFaces[3].set(4, 6, 7, 0);

        mesh->pFaces[4].set(8, 9, 10, 0);
        mesh->pFaces[5].set(8, 10, 11, 0);

        mesh->pFaces[6].set(12, 13, 14, 0);
        mesh->pFaces[7].set(12, 14, 15, 0);

        mesh->pFaces[8].set(16, 17, 18, 0);
        mesh->pFaces[9].set(16, 18, 19, 0);

        mesh->pFaces[10].set(20, 21, 22, 0);
        mesh->pFaces[11].set(20, 22, 23, 0);

        this->m_pMeshs.push_back(*mesh);
        this->m_iNumMeshs = 1;

        this->scale.x = 10;
        this->scale.y = 10;
        this->scale.z = 10;

        this->calculateNormal();
    }

    void loadTexture(QString file){
        stMaterial m;
        this->GenerateTexture(&m, file);
        this->m_pMaterials.clear();
        this->m_pMaterials.push_back(m);
        this->m_iNumMaterials = 1;

        this->compile();
    }
};

#endif // TILEMODEL_H
