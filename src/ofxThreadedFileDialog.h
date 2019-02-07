#pragma once


#include "ofMain.h"
#include <atomic>

#include "tinyfiledialogs.h"

struct ofxThreadedFileDialogResponse{
    string      id;
    string      filepath;
};

class ofxThreadedFileDialog: public ofThread{

public:
    ~ofxThreadedFileDialog(){
        stop();
        waitForThread(false);
    }

    void setup(){
        openDialogFinished  = true;
        saveDialogFinished  = true;

        openDialogLoaded    = false;
        saveDialogLoaded    = false;

        loadingOpenCounter      = 0;
        loadedOpenCounter       = 0;

        loadingSaveCounter      = 0;
        loadedSaveCounter       = 0;

        openedFileEvent     = false;
        savedFileEvent      = false;

        tempID              = "";
        tempTitle           = "";
        tempFileName        = "";

        startThread();
    }

    void openFile(string id, string title){
        tempID              = id;
        tempTitle           = title;
        openDialogFinished  = false;
    }

    void saveFile(string id, string title,string filename){
        tempID              = id;
        tempTitle           = title;
        tempFileName        = filename;
        saveDialogFinished  = false;
    }

    void stop(){
        std::unique_lock<std::mutex> lck(mutex);
        stopThread();
        condition.notify_all();
    }

    bool getIsOpenFileLoaded(){
        return openDialogLoaded;
    }

    bool getIsSaveFileLoaded(){
        return saveDialogLoaded;
    }

    bool getHaveNewOpenFile(){
        if(loadingOpenCounter==loadedOpenCounter && openedFileEvent){
            openedFileEvent = false;
            return true;
        }else{
            return false;
        }
    }

    bool getHaveNewSaveFile(){
        if(loadingSaveCounter==loadedSaveCounter && savedFileEvent){
            savedFileEvent = false;
            return true;
        }else{
            return false;
        }
    }

    string getLastFile(){
        return lastFile;
    }

    void threadedFunction(){
        while(isThreadRunning()){
            if(!openDialogFinished && saveDialogFinished){
                openDialogFinished = true;
                openDialogLoaded = true;
                loadingOpenCounter++;
                char const * filepath = tinyfd_openFileDialog(tempTitle.c_str(),"",0,NULL,NULL,0);
                if(filepath){
                    ofFile file(filepath);
                    if (file.exists()){
                        lastFile = file.getAbsolutePath();
                    }
                }else{
                    lastFile = "";
                }

                openDialogLoaded = false;
                openedFileEvent = true;
                loadedOpenCounter++;

                ofxThreadedFileDialogResponse temp;
                temp.id = tempID;
                temp.filepath = lastFile;

                ofNotifyEvent( fileDialogEvent, temp, this );

                condition.notify_all();
            }
            if(!saveDialogFinished && openDialogFinished){
                saveDialogFinished = true;
                saveDialogLoaded = true;
                loadingSaveCounter++;
                char const * filepath = tinyfd_saveFileDialog(tempTitle.c_str(),tempFileName.c_str(),0,NULL,NULL);
                if(filepath){
                    ofFile newFile(filepath);
                    lastFile = newFile.getAbsolutePath();
                }else{
                    lastFile = "";
                }

                saveDialogLoaded = false;
                savedFileEvent = true;
                loadedSaveCounter++;

                ofxThreadedFileDialogResponse temp;
                temp.id = tempID;
                temp.filepath = lastFile;

                ofNotifyEvent( fileDialogEvent, temp, this );

                condition.notify_all();
            }
            sleep(10);
        }
    }

    ofEvent<ofxThreadedFileDialogResponse>         fileDialogEvent;

protected:
    std::condition_variable condition;
    string                  tempID;
    string                  lastFile;
    string                  tempTitle;
    string                  tempFileName;
    int                     loadingOpenCounter;
    int                     loadedOpenCounter;
    int                     loadingSaveCounter;
    int                     loadedSaveCounter;
    bool                    openedFileEvent;
    bool                    savedFileEvent;
    bool                    openDialogFinished;
    bool                    saveDialogFinished;
    bool                    openDialogLoaded;
    bool                    saveDialogLoaded;

};
