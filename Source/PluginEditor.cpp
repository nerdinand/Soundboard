/*
 ==============================================================================

 PluginEditor.cpp
 Author:  Daniel Lindenfelser

 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoundboardAudioProcessorEditor::SoundboardAudioProcessorEditor(
    SoundboardAudioProcessor& p)
    : AudioProcessorEditor(&p)
    , processor(p)
    , mPauseState(true)
    , mTimerState(true)
{
    addAndMakeVisible(topBar = new Bar());

    addAndMakeVisible(fadeOutSlider = new Slider());
    fadeOutSlider->setRange(1, 30, 1);
    fadeOutSlider->setSkewFactor(0.5);
    fadeOutSlider->setValue(processor.getFadeOutSeconds());
    fadeOutSlider->setSliderStyle(Slider::SliderStyle::Rotary);
    fadeOutSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    fadeOutSlider->addListener(this);

    addAndMakeVisible(fadeOutLabel = new Label());
    fadeOutLabel->setColour(Label::ColourIds::textColourId, ThemeForeground1);
    fadeOutLabel->setText("Ausblendzeit: " + String(processor.getFadeOutSeconds()) + "s", NotificationType::dontSendNotification);

    addAndMakeVisible(loadDirectoryButton = new AwesomeButton(FA_FOLDER_OPEN_O));
    loadDirectoryButton->setButtonText("Soundboard Laden");
    loadDirectoryButton->addListener(this);

    addAndMakeVisible(gridButton = new AwesomeButton(FA_TH));
    gridButton->setButtonText("Grid");
    gridButton->addListener(this);

    addAndMakeVisible(grid = new SoundboardGridComponent(p));
    grid->setVisible(false);
    addAndMakeVisible(table = new SoundboardTableComponent(p));

    addAndMakeVisible(settingsButton = new AwesomeButton(FA_COG));
    settingsButton->setButtonText("Einstellungen");
    settingsButton->addListener(this);

    addAndMakeVisible(oscActivityIndicator = new ActivityIndicator());
    oscActivityIndicator->setTitle(CharPointer_UTF8("OSC Aktivit\xc3\xa4t"));

    if (processor.wrapperType != AudioProcessor::wrapperType_Standalone)
    {
        addAndMakeVisible (resizer = new ResizableCornerComponent (this, &resizeLimits));
        resizeLimits.setSizeLimits (380, 320, 1024, 768);
    }

    refresh();

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(processor.getWindowWidth(), processor.getWindowHeight());

    startTimer(TimerIdBlink, (int)(1000 * 0.5));
    startTimer(TimerIdUpdate, 50);
    startTimer(TimerIdRefresh, (int)(1000 * 0.5));
}

SoundboardAudioProcessorEditor::~SoundboardAudioProcessorEditor()
{
    stopTimer(TimerIdBlink);
    stopTimer(TimerIdUpdate);
    stopTimer(TimerIdRefresh);
    topBar = nullptr;
    fadeOutSlider = nullptr;
    fadeOutLabel = nullptr;
    loadDirectoryButton = nullptr;
    table = nullptr;
    grid = nullptr;
    resizer = nullptr;
    settingsButton = nullptr;
    oscActivityIndicator = nullptr;
    gridButton = nullptr;
}

//==============================================================================
void SoundboardAudioProcessorEditor::paint(Graphics& g)
{
    g.fillAll(ThemeBackground1);
}

void SoundboardAudioProcessorEditor::resized()
{
    topBar->setBounds(0, 0, getWidth(), 64);

    fadeOutSlider->setBounds(3, 5, 24, 24);
    fadeOutLabel->setBounds(27, 5, 150, 24);

    loadDirectoryButton->setBounds(getWidth() - 153, 5, 150, 24);
    gridButton->setBounds(getWidth() - 220, 5, 64, 24);

    table->setBounds(0, 64, getWidth(), getHeight() - 64);
    grid->setBounds(0, 64, getWidth(), getHeight() - 64);

    if (resizer)
    {
        resizer->setBounds (getWidth () - 16, getHeight () - 16, 16, 16);
    }
    settingsButton->setBounds(getWidth() - 123, 32, 120, 24);
    oscActivityIndicator->setBounds(3, 32, 120, 24);

    processor.storeWindowWidth(getWidth());
    processor.storeWindowHeight(getHeight());
}

void SoundboardAudioProcessorEditor::buttonClicked(Button* buttonThatWasClicked)
{
    if (loadDirectoryButton == buttonThatWasClicked) {
        FileChooser chooser("Open...");
        if (chooser.browseForDirectory()) {
            File directory = chooser.getResult();
            if (directory.isDirectory()) {
                processor.openDirectory(directory);
                table->updateContent();
                grid->updateContent();
            }
        }
    }
    else if (settingsButton == buttonThatWasClicked) {
        processor.openSettings();
    }
    else if (gridButton == buttonThatWasClicked) {
        if (grid->isVisible()) {
            grid->setVisible(false);
            table->setVisible(true);
            table->toFront(true);
            static int theme = 1;
            if (theme >= 6)
                theme = 1;
            Themes val = static_cast<Themes>(theme);
            processor.SwitchTheme(val);
            theme ++;
        }
        else {
            table->setVisible(false);
            grid->setVisible(true);
            grid->toFront(true);
        }
    }
}

void SoundboardAudioProcessorEditor::timerCallback(int timerID)
{
    if (timerID == TimerIdUpdate) {
        fadeOutSlider->setValue(processor.getFadeOutSeconds(), NotificationType::dontSendNotification);
        fadeOutLabel->setText("Ausblendzeit: " + String(processor.getFadeOutSeconds()) + "s",
                              NotificationType::dontSendNotification);

        if (oscActivityIndicator->getActive() != processor.receivedOscMessages()) {
            oscActivityIndicator->setActive(processor.receivedOscMessages());
            //repaint();
        }
    }
    else if (timerID == TimerIdRefresh) {
        refresh();
    }
}

void SoundboardAudioProcessorEditor::refresh()
{
    if (!processor.isLoocked()) {
        table->updateContent();
    }
}

void SoundboardAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == fadeOutSlider) {
        processor.setFadeOutSeconds((int)fadeOutSlider->getValue());
        fadeOutLabel->setText("Ausblendzeit: " + String(processor.getFadeOutSeconds()) + "s",
                              NotificationType::dontSendNotification);
    }
    else {
        int index = slider->getName().getIntValue();
        processor.setGain(index, (float)slider->getValue());
    }
}
