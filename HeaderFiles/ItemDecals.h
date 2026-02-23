#ifndef ITEM_DECALS
#define ITEM_DECALS

#include"UIPanel.h"

class ItemDecals : public UIPanel {
    public:
        ItemDecals(Texture& itemAtlas);
        
        void UploadDecalData();

        std::vector<UIVertex> selectedItemVertices;

};

#endif