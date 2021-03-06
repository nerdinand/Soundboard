/*
 ==============================================================================

 PluginProcessor.h
 Author:  Daniel Lindenfelser

 ==============================================================================
 */

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Player.h"
#include "OscSettings.h"

class LookAndFeel_Ultraschall;

static const Identifier DirectoryIdentifier("Directory");
static const Identifier CurrentDirectoryIdentifier("CurrentDirectory");
static const Identifier CurrentProgramIndexIdentifier("CurrentProgramIndex");

static const Identifier WindowWidthIdentifier("WindowWidth");
static const Identifier WindowHeightIdentifier("WindowHeight");

class SoundboardAudioProcessor : public AudioProcessor,
                                 public ChangeListener,
                                 public OscMessageListener,
                                 public MultiTimer {
public:
    SoundboardAudioProcessor();
    ~SoundboardAudioProcessor();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    int getNumParameters() override;
    float getParameter(int index) override;
    void setParameter(int index, float newValue) override;
    const String getParameterName(int index) override;
    const String getParameterText(int index) override;

    const String getInputChannelName(int channelIndex) const override;
    const String getOutputChannelName(int channelIndex) const override;
    bool isInputChannelStereoPair(int index) const override;
    bool isOutputChannelStereoPair(int index) const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;

    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const String& newName) override;

    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    int numAudioFiles();
    Player* SamplePlayerAtIndex(int index);
    void openDirectory(File directory);
    bool isLoocked() { return playersLock; }

    int getFadeOutSeconds();
    void setFadeOutSeconds(int seconds);
    void openSettings();

    // ChangeListener
    void changeListenerCallback(ChangeBroadcaster* source);

    // OscMessageListener
    void handleOscMessage(osc::ReceivedPacket packet) override;
    void oscSendPlayerState(int index);
    void oscSendPlayerConfig(int index);
    void oscSendPlayerUpdate();
    void oscSendReset();

    // MultiTimer
    void timerCallback(int timerID);

    // OSC
    OscServer* getOscServer() { return oscServer; }
    bool receivedOscMessages() { return oscReceived > 0; }

    // Parameter Helper
    void setGain(int playerIndex, float value);
    int getWindowWidth();
    void storeWindowWidth(int width);
    int getWindowHeight();
    void storeWindowHeight(int height);

private:
    // Maximum Number of Sampler Slots
    static const int MaximumSamplePlayers = 50;

    // Init Program Number
    static const int ProgramNumberInit = 0;
    static const int ProgramNumberCustom = 255;

    // Timer Ids
    static const int TimerOscServerDelay = 0;
    static const int TimerOscRefresh = 1;
    static const int TimerMidiEvents = 2;
    ScopedPointer<LookAndFeel> mLookAndFeel;

    // Audio IO
    OwnedArray<Player> samplePlayers;
    AudioFormatManager formatManager;
    AudioSourceChannelInfo sourceChannelInfo;
    MixerAudioSource mixerAudioSource;
    bool playersLock;

    // Global Parameter
    enum GlobalParameter {
        GlobalParameterFadeOut = 0,
        GlobalParameterCount = 1
    };

    int fadeOutSeconds;
    NormalisableRange<float> fadeOutRange;

    // Player Parameter
    enum PlayerParameter {
        PlayerParameterGain = 0,
        PlayerParameterCount = 1
    };

    // Settings
    int currentProgramIndex;
    String currentDirectory;
    ScopedPointer<PropertySet> fallbackProperties;
    ScopedPointer<PropertiesFile> propertiesFile;

    // OSC
    ScopedPointer<OscServer> oscServer;
    int oscReceived;

    // Settings
    ScopedPointer<OscSettings> settingsComponent;

    // MIDI
    MidiBuffer midiBuffer;
    CriticalSection midiCriticalSection;
    enum MidiFunction {
        PlayStop = 0,
        PlayPause = 1,
        PlayFadeOut = 2,
        HoldAndPlay = 3
    };
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SoundboardAudioProcessor)
};

#endif // PLUGINPROCESSOR_H_INCLUDED
