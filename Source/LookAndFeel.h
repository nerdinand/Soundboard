/*
  ==============================================================================

    LookAndFeel.h
    Author:  Daniel Lindenfelser

  ==============================================================================
*/

#ifndef LOOKANDFEEL_H_INCLUDED
#define LOOKANDFEEL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class LookAndFeel_Ultraschall : public LookAndFeel_V3 {
public:
    void drawTableHeaderBackground(Graphics &g, TableHeaderComponent &header) override;

    void drawTableHeaderColumn(Graphics &g, const String &columnName, int /*columnId*/,
            int width, int height, bool isMouseOver, bool isMouseDown,
            int columnFlags) override;

    void fillTextEditorBackground(Graphics &g, int /*width*/, int /*height*/, TextEditor &textEditor) override;
};

#endif  // LOOKANDFEEL_H_INCLUDED