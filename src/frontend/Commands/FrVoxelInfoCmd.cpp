#include "FrVoxelInfoCmd.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrBaseView.h"
#include "FrMosaicView.h"
#include "FrSliceView.h"
#include "FrOrthoView.h"
#include "FrTabSettingsDocObj.h"
#include "Fr2DSliceActor.h"
#include "FrUtils.h"
#include "FrVoxelInfoWidget.h"
#include "FrLayeredImage.h"

#include "vtkPointPicker.h"
#include "vtkCoordinate.h"
#include "vtkRenderer.h"


//Some defines
#define INVALIDE_RENDERER_NUM -1


FrVoxelInfoCmd::FrVoxelInfoCmd()
: m_isMouseXY(false), m_Action(FrVoxelInfoCmd::Undefined), m_PointPicker(0) {
    
}

void FrVoxelInfoCmd::SetMouseXY(int x, int y){
    m_mouseX = x; m_mouseY = y;
    m_isMouseXY = true;
}

bool FrVoxelInfoCmd::Execute(){
    if(!this->GetMainController() ||
        m_Action == FrVoxelInfoCmd::Undefined ||
        !m_isMouseXY) return false;

    bool result = false;
    switch(m_Action){
        case Update:
            result = UpdateVoxelInfo();
            break;
        case Reset:
            result = ResetVoxelInfo();
            break;
        default:
            // Do nothing
            break;
    }
    return result;
}

bool FrVoxelInfoCmd::UpdateVoxelInfo(){
	// get renderer    
	FrMainWindow* mv = this->GetMainController()->GetMainView();
	FrMainDocument* md = this->GetMainController()->GetMainDocument();
	FrTabSettingsDocObj* ts = md->GetCurrentTabSettings();
	
	std::vector<vtkRenderer*> renCollection;
	
	FrTabSettingsDocObj::View view = ts->GetActiveView();
	switch(view){
		case FrTabSettingsDocObj::View::SliceView:
			mv->GetSliceView()->GetImage()->GetRenderers(renCollection);
			break;
		case FrTabSettingsDocObj::View::MosaicView:
			mv->GetMosaicView()->GetImage()->GetRenderers(renCollection);
			break;
		case FrTabSettingsDocObj::View::OrthoView:
			{
				FrOrthoView* ov =  mv->GetOrthoView();

				// Find Image where click's occured
				int imgNumber = -1;
				for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
					if (ov->GetImage(i)->GetRenderer()->IsInViewport(m_mouseX, m_mouseY)){
						imgNumber = i; break;
					}
				}
				if (imgNumber != -1){
					ov->GetImage(imgNumber)->GetRenderers(renCollection);
				}
				else{
					ResetVoxelInfo();
					return false;
				}
			}
			break;	
	}

	vtkRenderer* renderer = renCollection[0];	// get renderer from current layer (from main layer atm)
	
	if (!m_PointPicker->Pick(m_mouseX, m_mouseY, 0, renderer)) {
        ResetVoxelInfo();
		return false;
    }	

    // Get the mapped position of the mouse using the picker.
    double ptMapped[3];
    m_PointPicker->GetMapperPosition(ptMapped);

    // We have to manually set the physical z-coordinate which requires
    // us to get the volume spacing.
    double dSpacing[3];

	// Get a shortcut to the pixel data.
    vtkImageData* pImageData = mv->GetSliceView()->GetImage()->GetLayerByID(0)->GetInput();		// get image data
    	
	pImageData->GetSpacing(dSpacing);			
	int slice = md->GetCurrentTabSettings()->GetSliceViewSettings()->SliceNumber;	// get slice number
    ptMapped[2] = 0;//slice*dSpacing[2];					

    // Get the volume index within the entire volume now.
    int nVolIdx = pImageData->FindPoint(ptMapped);

    // We have to handle different number of scalar components.
    unsigned short usPix;
    unsigned char usPixR;
    unsigned char usPixG;
    unsigned char usPixB;

	VoxelData vd;
	vd.name = "test";			// get layer (image?) name
	vd.timepoint = 44;			// get timepoint

	vd.Index[0] = ptMapped[0];			
	vd.Index[1] = ptMapped[1];
	vd.Index[2] = slice;							// set current slice number

	vd.Position[0] = ptMapped[0]*dSpacing[2];	
	vd.Position[1] = ptMapped[1]*dSpacing[2];
	vd.Position[2] = slice*dSpacing[2];							

	// Get voxel index, position
	// this should be done for all layers
    std::vector<FrLayerSettings*> layers;
    GetLayerSettings(md->GetCurrentTabSettings()->GetSliceViewSettings(), layers);
	
    std::vector<FrLayerSettings*>::iterator it, itEnd(layers.end());
    for (it = layers.begin(); it != itEnd; ++it){
        // get image data
        pImageData = mv->GetSliceView()->GetImage()->
            GetLayerByID( (*it)->ID )->GetInput();		

		switch (pImageData->GetNumberOfScalarComponents()) {
			case 1:
				{
					unsigned char* pPix = (unsigned char*)pImageData->GetScalarPointer();
					usPix = pPix[nVolIdx];
				//    FmtStr(m_strDetails, "Pixel val = [" << usPix << "]   at index [" << nVolIdx << "],  coordinates(" << ptMapped[0] << "," << ptMapped[1] << ")");
				}
				break;
	        
			case 3:
				{
					unsigned char* pPix = (unsigned char*)pImageData->GetScalarPointer();
					usPixR = pPix[nVolIdx * 3 + 0];
					usPixG = pPix[nVolIdx * 3 + 1];
					usPixB = pPix[nVolIdx * 3 + 2];

					usPix = (usPixR + usPixG + usPixB)/3;
				   // FmtStr(m_strDetails, "Pixel val = [" << (int)usPixR << "," << (int)usPixG << "," << (int)usPixB << "]   at index [" << nVolIdx << "],  coordinates(" << ptMapped[0] << "," << ptMapped[1] << ")");
				}
				break;

			default:
				//FmtStr(m_strDetails, "Unsupported number of scalar components [" << pImageData->GetNumberOfScalarComponents() << "] for pixel picker.");
				break;
		}
		
		// Set table values
		Voxel v;
        v.name = (*it)->Name;
        
        // get voxel value from current layer
		v.value = usPix;
		vd.voxels.push_back(v);
	}

	mv->GetVoxelInfoWidget()->SetVoxelData(vd);
	
    return false;
}

bool FrVoxelInfoCmd::ResetVoxelInfo(){
	// reset main properties
	FrMainWindow* mv = this->GetMainController()->GetMainView();
	mv->GetVoxelInfoWidget()->Clear();	// clear table of voxel values

    return false;
}

void FrVoxelInfoCmd::GetVoxelInfo(){
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrVoxelInfoCmd::CanUndo(){
    return false;
}

bool FrVoxelInfoCmd::Undo(){
    return false;
}

bool FrVoxelInfoCmd::Redo(){
    return false;
}