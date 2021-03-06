/*
  ==============================================================================

    OscSettings.h
    Created: 30 Nov 2014 9:59:12am
    Author:  Daniel Lindenfelser

  ==============================================================================
*/

#ifndef OSCSETTINGS_H_INCLUDED
#define OSCSETTINGS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "LookAndFeel.h"

static const Identifier OscReciveEnabledIdentifier("OscReciveEnabled");
static const Identifier OscRecivePortNumberIdentifier("OscRecivePortNumber");

static const Identifier OscRemoteEnabledIdentifier("OscRemoteEnabled");
static const Identifier OscRemoteHostnameIdentifier("OscRemoteHostname");
static const Identifier OscRemotePortNumberIdentifier("OscRemotePortNumber");
static const Identifier OscRemoteIsTouchOscIdentifier("OscRemoteIsTouchOsc");

//==============================================================================
/*
*/
class OscSettings : public Component,
                    public Button::Listener,
                    public TextEditor::Listener,
                    public OscMessageLogger,
                    public ListBoxModel {
public:
    OscSettings(PropertiesFile* pf, OscServer* s)
        : propertiesFile(pf)
        , oscServer(s)
    {
        addAndMakeVisible(topBar = new Bar());
        addAndMakeVisible(titleLabel = new Label());
        titleLabel->setText("OSC Einstellungen", NotificationType::dontSendNotification);
        titleLabel->setColour(Label::ColourIds::textColourId, Colours::grey);
        titleLabel->setFont(Font(24.0f * 0.7f));

        addAndMakeVisible(seperator = new Bar());

        addAndMakeVisible(localEnabled = new ToggleButton("receiveEnabled"));
        localEnabled->setButtonText("Receive on port:");
        localEnabled->addListener(this);
        localEnabled->setColour(ToggleButton::ColourIds::textColourId,
                                  Colours::grey);
        localEnabled->setToggleState(
            propertiesFile->getBoolValue(OscRecivePortNumberIdentifier.toString()),
            NotificationType::dontSendNotification);

        addAndMakeVisible(remoteEnabled = new ToggleButton("sendEnabledButton"));
        remoteEnabled->setButtonText("Send on port:");
        remoteEnabled->addListener(this);
        remoteEnabled->setColour(ToggleButton::ColourIds::textColourId,
                               Colours::grey);
        remoteEnabled->setToggleState(
            propertiesFile->getBoolValue(OscRemoteEnabledIdentifier.toString()),
            NotificationType::dontSendNotification);

        addAndMakeVisible(oscLoggerEnabled = new ToggleButton("oscLoggerEnabled"));
        oscLoggerEnabled->setButtonText("OSC Logger");
        oscLoggerEnabled->addListener(this);
        oscLoggerEnabled->setColour(ToggleButton::ColourIds::textColourId,
                                    Colours::grey);

        addAndMakeVisible(oscLogger = new ListBox("oscLogger"));
        oscLogger->setColour(ListBox::ColourIds::backgroundColourId, Colour::fromRGB(44, 44, 44));
        oscLogger->setColour(ListBox::ColourIds::outlineColourId, Colour::fromRGB(44, 44, 44));
        oscLogger->setColour(ListBox::ColourIds::textColourId, Colours::whitesmoke);
        oscLogger->setModel(this);

        addAndMakeVisible(localPortNumber = new TextEditor("receivePortNumber"));
        localPortNumber->setMultiLine(false);
        localPortNumber->setReturnKeyStartsNewLine(false);
        localPortNumber->setReadOnly(localEnabled->getToggleState());
        localPortNumber->setScrollbarsShown(true);
        localPortNumber->setCaretVisible(true);
        localPortNumber->setPopupMenuEnabled(true);
        localPortNumber->setText(
            propertiesFile->getValue(OscRecivePortNumberIdentifier.toString(), ""));
        localPortNumber->addListener(this);
        localPortNumber->setColour(TextEditor::ColourIds::textColourId,
                                     Colours::black);
        localPortNumber->setColour(TextEditor::ColourIds::backgroundColourId,
                                     Colours::grey.brighter());
        localPortNumber->setColour(TextEditor::ColourIds::focusedOutlineColourId,
                                     Colours::grey.brighter().brighter());
        localPortNumber->setColour(TextEditor::ColourIds::outlineColourId,
                                     Colours::grey);

        addAndMakeVisible(remotePortNumber = new TextEditor("sendPortNumber"));
        remotePortNumber->setMultiLine(false);
        remotePortNumber->setReturnKeyStartsNewLine(false);
        remotePortNumber->setReadOnly(remoteEnabled->getToggleState());
        remotePortNumber->setScrollbarsShown(true);
        remotePortNumber->setCaretVisible(true);
        remotePortNumber->setPopupMenuEnabled(true);
        remotePortNumber->setText(
            propertiesFile->getValue(OscRemotePortNumberIdentifier.toString(), ""));
        remotePortNumber->addListener(this);
        remotePortNumber->setColour(TextEditor::ColourIds::textColourId,
                                    Colours::black);
        remotePortNumber->setColour(TextEditor::ColourIds::backgroundColourId,
                                    Colours::grey.brighter());
        remotePortNumber->setColour(TextEditor::ColourIds::focusedOutlineColourId,
                                    Colours::grey.brighter().brighter());
        remotePortNumber->setColour(TextEditor::ColourIds::outlineColourId,
                                    Colours::grey);

        addAndMakeVisible(localHostLabel = new Label("receiveHostLabel", "Host:"));
        localHostLabel->setFont(Font(15.00f, Font::plain));
        localHostLabel->setJustificationType(Justification::centredLeft);
        localHostLabel->setEditable(false, false, false);
        localHostLabel->setColour(Label::textColourId, Colours::grey);

        addAndMakeVisible(localHonstname = new TextEditor("receiveHonstname"));
        localHonstname->setMultiLine(false);
        localHonstname->setReturnKeyStartsNewLine(false);
        localHonstname->setReadOnly(true);
        localHonstname->setScrollbarsShown(true);
        localHonstname->setCaretVisible(true);
        localHonstname->setPopupMenuEnabled(true);
        localHonstname->setText(SystemStats::getComputerName());
        localHonstname->setColour(TextEditor::ColourIds::textColourId,
                                    Colours::black);
        localHonstname->setColour(TextEditor::ColourIds::backgroundColourId,
                                    Colours::grey.brighter());
        localHonstname->setColour(TextEditor::ColourIds::focusedOutlineColourId,
                                    Colours::grey.brighter().brighter());
        localHonstname->setColour(TextEditor::ColourIds::outlineColourId,
                                    Colours::grey);

        addAndMakeVisible(remoteHonstname = new TextEditor("sendHonstname"));
        remoteHonstname->setMultiLine(false);
        remoteHonstname->setReturnKeyStartsNewLine(false);
        remoteHonstname->setReadOnly(remoteEnabled->getToggleState());
        remoteHonstname->setScrollbarsShown(true);
        remoteHonstname->setCaretVisible(true);
        remoteHonstname->setPopupMenuEnabled(true);
        remoteHonstname->setText(
            propertiesFile->getValue(OscRemoteHostnameIdentifier.toString(), ""));
        remoteHonstname->addListener(this);
        remoteHonstname->setColour(TextEditor::ColourIds::textColourId,
                                   Colours::black);
        remoteHonstname->setColour(TextEditor::ColourIds::backgroundColourId,
                                   Colours::grey.brighter());
        remoteHonstname->setColour(TextEditor::ColourIds::focusedOutlineColourId,
                                   Colours::grey.brighter().brighter());
        remoteHonstname->setColour(TextEditor::ColourIds::outlineColourId,
                                   Colours::grey);

        addAndMakeVisible(remoteHostLabel = new Label("sendHostLabel", "Device:"));
        remoteHostLabel->setFont(Font(15.00f, Font::plain));
        remoteHostLabel->setJustificationType(Justification::centredLeft);
        remoteHostLabel->setEditable(false, false, false);
        remoteHostLabel->setColour(Label::textColourId, Colours::grey);

        addAndMakeVisible(buttomBar = new Bar());
        addAndMakeVisible(closeButton = new AwesomeButton(FA_CHECK));

        closeButton->setButtonText("Ok");
        closeButton->addListener(this);

        setSize(600, 400);
    }

    ~OscSettings()
    {
        titleLabel = nullptr;
        localEnabled = nullptr;
        remoteEnabled = nullptr;
        oscLoggerEnabled = nullptr;
        oscLogger = nullptr;
        localPortNumber = nullptr;
        remotePortNumber = nullptr;
        localHostLabel = nullptr;
        localHonstname = nullptr;
        remoteHonstname = nullptr;
        remoteHostLabel = nullptr;
        closeButton = nullptr;
        buttomBar = nullptr;
        topBar = nullptr;
        seperator = nullptr;
    }

    void paint(Graphics& g)
    {
        g.fillAll(Colour::fromRGB(62, 62, 62)); // clear the background
    }

    void resized()
    {
        topBar->setBounds(0, 0, getWidth(), 32);
        titleLabel->setBounds(3, 5, getWidth(), 24);

        localEnabled->setBounds(8, 38, 136, 24);
        remoteEnabled->setBounds(8, 65, 136, 24);

        localPortNumber->setBounds(152, 38, 48, 24);
        remotePortNumber->setBounds(152, 65, 48, 24);

        localHostLabel->setBounds(208, 38, 56, 24);
        localHonstname->setBounds(272, 38, 150, 24);

        remoteHonstname->setBounds(272, 65, 150, 24);
        remoteHostLabel->setBounds(208, 65, 56, 24);

        oscLoggerEnabled->setBounds(8, 96, getWidth() - 16, 24);
        seperator->setBounds(0, 94, getWidth(), 28);

        oscLogger->setBounds(0, 122, getWidth(), getHeight() - 122 - 32);

        closeButton->setBounds(getWidth() - 123, getHeight() - 27, 120, 24);
        buttomBar->setBounds(0, getHeight() - 32, getWidth(), 32);
    }

    void buttonClicked(Button* buttonThatWasClicked)
    {
        if (buttonThatWasClicked == localEnabled) {
            localPortNumber->setReadOnly(localEnabled->getToggleState());
            propertiesFile->setValue(OscReciveEnabledIdentifier.toString(),
                                     localEnabled->getToggleState());
        }
        else if (buttonThatWasClicked == remoteEnabled) {
            remoteHonstname->setReadOnly(remoteEnabled->getToggleState());
            remotePortNumber->setReadOnly(remoteEnabled->getToggleState());
            propertiesFile->setValue(OscRemoteEnabledIdentifier.toString(),
                                     remoteEnabled->getToggleState());
        }
        else if (buttonThatWasClicked == closeButton) {
            oscServer->removeLogger();
            this->getParentComponent()->removeChildComponent(this);
        }
        else if (buttonThatWasClicked == oscLoggerEnabled) {
            if (oscLoggerEnabled->getToggleState()) {
                oscServer->setLogger(this);
            }
            else {
                oscServer->removeLogger();
            }
        }
    }

    void textEditorTextChanged(TextEditor& editor)
    {
        if (&editor == localPortNumber) {
            propertiesFile->setValue(OscRecivePortNumberIdentifier.toString(),
                                     localPortNumber->getText().getIntValue());
        }
        else if (&editor == remotePortNumber) {
            propertiesFile->setValue(OscRemotePortNumberIdentifier.toString(),
                                     remotePortNumber->getText().getIntValue());
        }
        else if (&editor == remoteHonstname) {
            propertiesFile->setValue(OscRemoteHostnameIdentifier.toString(),
                                     remoteHonstname->getText());
        }
    }

    void logOscMessage(juce::String message)
    {
        oscLoggerBuffer.add(message);
        if (oscLoggerBuffer.size() >= 50)
            oscLoggerBuffer.remove(0);
        oscLogger->updateContent();
        oscLogger->scrollToEnsureRowIsOnscreen(oscLoggerBuffer.size());
        oscLogger->repaint();
    }

    int getNumRows() { return oscLoggerBuffer.size(); }

    void paintListBoxItem(int rowNumber, Graphics& g, int width, int height,
                          bool /*rowIsSelected*/)
    {
        g.setColour(Colours::grey);
        g.drawText(oscLoggerBuffer[rowNumber], 0, 0, width, height,
                   Justification::centredLeft);
    }

private:
    ScopedPointer<ToggleButton> localEnabled;
    ScopedPointer<TextEditor> localPortNumber;
    ScopedPointer<Label> localHostLabel;
    ScopedPointer<TextEditor> localHonstname;

    ScopedPointer<ToggleButton> remoteEnabled;
    ScopedPointer<TextEditor> remotePortNumber;
    ScopedPointer<Label> remoteHostLabel;
    ScopedPointer<TextEditor> remoteHonstname;

    ScopedPointer<ToggleButton> oscLoggerEnabled;
    ScopedPointer<ListBox> oscLogger;

    StringArray oscLoggerBuffer;

    ScopedPointer<Label> titleLabel;
    ScopedPointer<AwesomeButton> closeButton;
    ScopedPointer<Bar> buttomBar;
    ScopedPointer<Bar> topBar;
    ScopedPointer<Bar> seperator;

    PropertiesFile* propertiesFile;
    OscServer* oscServer;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscSettings)
};

#endif // OSCSETTINGS_H_INCLUDED
