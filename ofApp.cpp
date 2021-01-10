#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("openFrameworks");
	ofSetFrameRate(60);
	
	ofBackground(239);
	ofSetColor(39);
	ofNoFill();

	this->near_line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
	this->line_list.clear();
	this->near_line.clear();

	int len = 500;
	
	{
		ofMesh line;
		line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
		for (int param = 0; param < 100; param += 1) {

			line.addVertex(glm::vec3(this->make_point(len, param), 0));

			if (param > 0) {

				line.addIndex(line.getNumVertices() - 1);
				line.addIndex(line.getNumVertices() - 2);
			}
		}
		this->line_list.push_back(line);
	}

	for (int i = 0; i < 5; i++) {

		ofMesh line;
		line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
		auto noise_seed = ofRandom(1000);
		for (int x = -len * 0.5; x <= len * 0.5; x += 5) {

			auto y = ofMap(ofNoise(noise_seed, x * 0.0035, ofGetFrameNum() * 0.0035), 0, 1, -len * 0.5, len * 0.5);
			line.addVertex(glm::vec3(x, y, 0));

			if (x > len * -0.5) {

				line.addIndex(line.getNumVertices() - 1);
				line.addIndex(line.getNumVertices() - 2);
			}
		}

		this->line_list.push_back(line);
	}

	for (int i = 0; i < this->line_list.size(); i++) {

		for (int k = i + 1; k < this->line_list.size(); k++) {

			for (int m = 0; m < this->line_list[i].getNumVertices(); m++) {

				bool m_flag = false;
				int m_index = m;
				for (int p = 0; p < this->line_list[k].getNumVertices(); p++) {

					if (glm::distance(this->line_list[i].getVertex(m), this->line_list[k].getVertex(p)) < 50) {

						if (m_flag == false) {

							m_index = this->near_line.getNumVertices();
							this->near_line.addVertex(this->line_list[i].getVertex(m));
							m_flag = true;
						}

						this->near_line.addVertex(this->line_list[k].getVertex(p));

						this->near_line.addIndex(m_index);
						this->near_line.addIndex(this->near_line.getNumVertices() - 1);
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
	
	this->near_line.draw();
	ofFill();
	for (auto& vertex : this->near_line.getVertices()) {

		ofDrawCircle(vertex, 3);
	}
	ofNoFill();
	ofDrawRectangle(glm::vec2(-250, -250), 500, 500);
}

//--------------------------------------------------------------
glm::vec2 ofApp::make_point(int len, int param) {

	param = param % 100;
	if (param < 25) {

		return glm::vec2(ofMap(param, 0, 25, -len * 0.5, len * 0.5), -len * 0.5);
	}
	else if (param < 50) {

		return glm::vec2(len * 0.5, ofMap(param, 25, 50, -len * 0.5, len * 0.5));
	}
	else if (param < 75) {

		return glm::vec2(ofMap(param, 50, 75, len * 0.5, -len * 0.5), len * 0.5);
	}
	else {

		return glm::vec2(-len * 0.5, ofMap(param, 75, 100, len * 0.5, -len * 0.5));
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}