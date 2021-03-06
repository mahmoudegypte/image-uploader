#ifndef ColorPickerTool_h__
#define ColorPickerTool_h__

#include "../DrawingElement.h"
#include "../MovableElement.h"
#include "AbstractDrawingTool.h"
#include <stdint.h>
#include "3rdpart/GdiplusH.h"
namespace ImageEditor {

class  ColorPickerTool : public AbstractDrawingTool {
public:
    ColorPickerTool( Canvas* canvas );
    void beginDraw( int x, int y );
    void continueDraw( int x, int y, DWORD flags );
    void endDraw( int x, int y );
    void render( Painter* gr );
    virtual CursorType getCursor(int x, int y);

    virtual void rightButtonClick(int x, int y);

};
}

#endif // ColorPickerTool_h__