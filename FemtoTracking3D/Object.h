
#ifndef PX_OBJECT_H
#define PX_OBJECT_H

#include <iostream>
#include <string>
#include <chrono>
#include "Serializable.h"

namespace px {

class Object : public Serializable {
public:
	Object();
	Object(const std::string& name, bool bVisible = true, bool bSelected = false);
	virtual ~Object();

	bool serialize(BinaryFileWriter& out, SerializeType type = SERIALIZE_DEFAULT);
	bool deserialize(BinaryFileReader& in, SerializeType type = SERIALIZE_DEFAULT);

	virtual void setName(const std::string& name);
	virtual void setVisible(bool b);
	virtual void show();
	virtual bool hide();
	virtual void setSelected(bool b);
	virtual void select();
	virtual void deselect();
	virtual bool isVisible() const;
	virtual bool isSelected() const;
	virtual std::string getName() const;

	template <typename ChronoType = std::chrono::microseconds>
	std::size_t timestamp();
	bool setTimestamp(std::size_t timestamp);
	virtual std::size_t getTimestamp() const;

	virtual std::string toString() const;

protected:
	std::string name;
	std::atomic<std::size_t> stamp;
	bool bVisible;
	bool bSelected;
};

template <typename ChronoType>
std::size_t Object::timestamp() {
	auto time = std::chrono::duration_cast<ChronoType>(std::chrono::system_clock::now().time_since_epoch());
	this->stamp = time.count();
	return this->stamp;
}

}

#endif
