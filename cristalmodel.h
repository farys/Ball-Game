#ifndef CRISTALMODEL_H
#define CRISTALMODEL_H
#include "c3ds.h"

class CristalModel: protected C3DS {
public:
    CristalModel(){
        stMesh *mesh = new stMesh;

        /* vertexs */
        mesh->pVerts = new stVert[6];
        mesh->pNormalVerts = new stVert[6];
        mesh->pTexs = new stTex[6];
        mesh->iNumVerts = 6;
        mesh->pVerts[0].set(0, 1, 0); mesh->pTexs[0].set(0.5, 1);
        mesh->pVerts[1].set(0, -1, 0); mesh->pTexs[1].set(0.5, 1);
        mesh->pVerts[2].set(0, 0, -1); mesh->pTexs[2].set(0, 0);
        mesh->pVerts[3].set(1, 0, 0); mesh->pTexs[3].set(1, 0);
        mesh->pVerts[4].set(0, 0, 1); mesh->pTexs[4].set(1, 0);
        mesh->pVerts[5].set(-1, 0, 0); mesh->pTexs[5].set(0, 0);

        /* faces */
        mesh->pFaces = new stFace[8];
        mesh->iNumFaces = 8;
        mesh->pFaces[0].set(2, 0, 3, 0);
        mesh->pFaces[1].set(3, 0, 4, 0);
        mesh->pFaces[2].set(4, 0, 5, 0);
        mesh->pFaces[3].set(5, 0, 2, 0);
        mesh->pFaces[4].set(5, 2, 1, 0);
        mesh->pFaces[5].set(2, 3, 1, 0);
        mesh->pFaces[6].set(3, 4, 1, 0);
        mesh->pFaces[7].set(4, 5, 1, 0);

        this->m_pMeshs.push_back(*mesh);
        this->m_iNumMeshs = 1;

        this->scale.x = 10;
        this->scale.y = 10;
        this->scale.z = 10;

        /* Materials */
        stMaterial m;
        strcpy(m.szTextureFile, "CRISTAL.JPG");

        this->GenerateTexture(&m);
        this->m_pMaterials.push_back(m);
        this->m_iNumMaterials = 1;

        this->calculateNormal();
        this->compile();

    }
};

#endif // CRISTALMODEL_H
