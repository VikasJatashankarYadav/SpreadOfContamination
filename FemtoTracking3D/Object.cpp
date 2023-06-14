#include "Object.h"
#include <sstream>

namespace px {

const static std::string DEFAULT_NAME = "Object";

Object::Object() {
	this->name = DEFAULT_NAME;
	this->bVisible = true;
	this->bSelected = false;
	this->stamp = this->timestamp();
}

Object::Object(const std::string& name, bool bVisible, bool bSelected) {
	this->name = name;
	this->bVisible = bVisible;
	this->bSelected = bSelected;
	this->stamp = this->timestamp();
}

Object::~Object() {}

bool Object::serialize(BinaryFileWriter& out, SerializeType type)  { return true; }
bool Object::deserialize(BinaryFileReader& in, SerializeType type) { return true; }

void Object::setName(const std::string& name) {
	this->name = name;
}

void Object::setVisible(bool b) {
	this->bVisible = b;
}

void Object::show() {
	this->bVisible = true;
}

bool Object::hide() {
	this->bVisible = false;
	return true;
}

void Object::setSelected(bool b) {
	this->bSelected = b;
}

void Object::select() {
	this->bSelected = true;
}

void Object::deselect() {
	this->bSelected = false;
}

std::string Object::getName() const {
	return this->name;
}

bool Object::isVisible() const {
	return this->bVisible;
}

bool Object::isSelected() const {
	return this->bSelected;
}

bool Object::setTimestamp(std::size_t timestamp) {
	this->stamp = timestamp;
	return true;
}

std::size_t Object::getTimestamp() const {
	return this->stamp;
}

std::string Object::toString() const {
	std::stringstream stream;
	auto YN = [] (bool b) { if ( b ) return "True"; else return "False"; };
	stream << '[' << this->name << " bVisible:" << YN(this->bVisible) << " bSelected:" << YN(this->bSelected) << " (" << this->stamp << ')';
	return stream.str();
}

}
