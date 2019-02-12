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
        notificationFinished        = true;
        openDialogFinished          = true;
        saveDialogFinished          = true;
        openDialogFolderFinished    = true;

        openDialogLoaded            = false;
        openFolderDialogLoaded      = false;
        saveDialogLoaded            = false;

        tempID              = "";
        tempTitle           = "";
        tempMessage         = "";
        tempFileName        = "";

        startThread();
    }

    void notificationPopup(string title, string message){
        tempTitle               = title;
        tempMessage             = message;
        notificationFinished    = false;
    }

    void openFile(string id, string title){
        tempID              = id;
        tempTitle           = title;
        openDialogFinished  = false;
    }

    void openFolder(string id, string title){
        tempID                      = id;
        tempTitle                   = title;
        openDialogFolderFinished    = false;
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

    bool getIsOpenFolderLoaded(){
        return openFolderDialogLoaded;
    }

    bool getIsSaveFileLoaded(){
        return saveDialogLoaded;
    }

    string getLastFile(){
        return lastFile;
    }

    void threadedFunction(){
        while(isThreadRunning()){
            if(!openDialogFinished && saveDialogFinished && openDialogFolderFinished){
                openDialogFinished = true;
                openDialogLoaded = true;
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

                if(lastFile != ""){
                    ofxThreadedFileDialogResponse temp;
                    temp.id = tempID;
                    temp.filepath = lastFile;

                    ofNotifyEvent( fileDialogEvent, temp, this );
                }

                condition.notify_all();
            }else if(!saveDialogFinished && openDialogFinished && openDialogFolderFinished){
                saveDialogFinished = true;
                saveDialogLoaded = true;
                char const * filepath = tinyfd_saveFileDialog(tempTitle.c_str(),tempFileName.c_str(),0,NULL,NULL);
                if(filepath){
                    ofFile newFile(filepath);
                    lastFile = newFile.getAbsolutePath();
                }else{
                    lastFile = "";
                }

                saveDialogLoaded = false;

                if(lastFile != ""){
                    ofxThreadedFileDialogResponse temp;
                    temp.id = tempID;
                    temp.filepath = lastFile;

                    ofNotifyEvent( fileDialogEvent, temp, this );
                }

                condition.notify_all();
            }else if(!openDialogFolderFinished && saveDialogFinished && openDialogFinished){
                openDialogFolderFinished = true;
                openFolderDialogLoaded = true;
                char const * filepath = tinyfd_selectFolderDialog(tempTitle.c_str(), NULL);
                if(filepath){
                    ofFile file(filepath);
                    if (file.exists() && file.isDirectory()){
                        lastFile = file.getAbsolutePath();
                    }
                }else{
                    lastFile = "";
                }

                openFolderDialogLoaded = false;

                if(lastFile != ""){
                    ofxThreadedFileDialogResponse temp;
                    temp.id = tempID;
                    temp.filepath = lastFile;

                    ofNotifyEvent( fileDialogEvent, temp, this );
                }
            }
            if(!notificationFinished){
                notificationFinished = true;
                tinyfd_notifyPopup(tempTitle.c_str(),tempMessage.c_str(), "info");
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
    string                  tempMessage;
    string                  tempFileName;
    bool                    notificationFinished;
    bool                    openDialogFinished;
    bool                    openDialogFolderFinished;
    bool                    saveDialogFinished;
    bool                    openDialogLoaded;
    bool                    openFolderDialogLoaded;
    bool                    saveDialogLoaded;

};
