#include "FrMainDocument.h"
#include "FrTBCFilter.h"
#include "FrTabSettingsDocObj.h"

FrMainDocument::FrMainDocument() 
: FrDocument(){
}

FrMainDocument::~FrMainDocument(){
}

void FrMainDocument::GetAllImages(std::vector<FrDocumentObj*>& images){
    images.clear();
    GetObjectsByType(images, FrDocumentObj::ImageObject);
}

void FrMainDocument::SetDefaultValues(){
    // Setup threshold/brightness/contrast values
    m_Threshold = DEF_THRESHOLD;
    m_Brightness = DEF_BRIGHTNESS;
    m_Contrast = DEF_CONTRAST;
	
	m_Slice = 0;
}

FrTabSettingsDocObj* FrMainDocument::GetCurrentTabSettings(){
    std::vector<FrDocumentObj*>& objects = this->GetObjects();
        
    FrTabSettingsDocObj* result = 0L;
    if(objects.size() > 0){

        std::vector<FrDocumentObj*>::iterator it, itEnd(objects.end());
        for(it = objects.begin(); it != itEnd; ++it){
            // Check for type 
            if((*it)->GetType() != FrDocumentObj::TabSettings) continue;
            
            // returns first current
            FrTabSettingsDocObj* ts = (FrTabSettingsDocObj*)(*it);
            if(ts->GetIsCurrent()){
                result = ts;
                break;
            }
        }
    }
    return result;
}