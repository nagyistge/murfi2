#ifndef FR_REFRESH_LAYER_INFO_CMD
#define FR_REFRESH_LAYER_INFO_CMD

// This makes user no need to include 
// those headers in command source file.
#include "FrBaseCmd.h"

// Base abstract class for commands.  Supports undo and redo
// features.  Command is called by Execute() method.
// You should derive all your commans from this.
class FrRefreshLayerInfoCmd : public FrBaseCmd {
public:
    // Constructor/destructor
    FrRefreshLayerInfoCmd();

	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();

    // Properties
    FrPropMacro(FrMainController*, MainController);

protected:
    void UpdatePipelineForID(int id, int point);
};

#endif // FR_REFRESH_LAYER_INFO_CMD