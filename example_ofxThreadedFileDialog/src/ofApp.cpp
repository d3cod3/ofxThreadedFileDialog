#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetCircleResolution(50);

    fileDialog.setup();

    tinyfd_notifyPopup("ofxThreadedFileDialog", "a non-blocking cross-platformfile dialog\nbased on http://tinyfiledialogs.sourceforge.net", "info");
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);

    ofSetColor(255);
    ofDrawCircle(ofGetWindowWidth()/2,ofGetWindowHeight()/2,ofRandom(100));

    std::ostringstream oss;
    oss << ofToString(ofGetFrameRate(), 2) << " fps" << endl;
    oss << "" << endl;
    oss << "[o]pen file dialog: " << (fileDialog.getIsOpenFileLoaded() ? "on" : "off") << endl;
    oss << "[f]open folder dialog: " << (fileDialog.getIsOpenFolderLoaded() ? "on" : "off") << endl;
    oss << "[s]ave file dialog: " << (fileDialog.getIsSaveFileLoaded() ? "on" : "off") << endl;
    ofSetColor(255);
    ofDrawBitmapStringHighlight(oss.str(), 10, 20);
}

//--------------------------------------------------------------
void ofApp::exit(){
    fileDialog.stop();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'o'){
        fileDialog.openFile("open file","open new file");
    }else if(key == 'f'){
        fileDialog.openFolder("open folder","open new folder");
    }else if(key == 's'){
        fileDialog.saveFile("save file","save file as","testing.txt");
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
void ofApp::onFileDialogResponse(ofxThreadedFileDialogResponse &response){
    if(response.id == "open file"){
        ofLog(OF_LOG_NOTICE,"opened file: %s", response.filepath.c_str());
    }else if(response.id == "open folder"){
        ofLog(OF_LOG_NOTICE,"opened folder: %s", response.filepath.c_str());
    }else if(response.id == "save file"){
        ofLog(OF_LOG_NOTICE,"saved file: %s", response.filepath.c_str());
    }
}
