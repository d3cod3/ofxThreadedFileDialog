#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetCircleResolution(50);

    fileDialog.setup();

    tinyfd_notifyPopup("ofxThreadedFileDialog", "a non-blocking cross-platformfile dialog\nbased on http://tinyfiledialogs.sourceforge.net", "info");
}

//--------------------------------------------------------------
void ofApp::update(){
    if(fileDialog.getHaveNewOpenFile()){
        ofLog(OF_LOG_NOTICE,"opened file: %s", fileDialog.getLastFile().c_str());
    }

    if(fileDialog.getHaveNewSaveFile()){
        ofLog(OF_LOG_NOTICE,"saved file: %s", fileDialog.getLastFile().c_str());
    }
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
        fileDialog.openFile("open new file");
    }else if(key == 's'){
        fileDialog.saveFile("save file as","testing.txt");
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
