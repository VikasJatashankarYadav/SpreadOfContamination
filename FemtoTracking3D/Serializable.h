
#ifndef PX_SERIALIZABLE_H
#define PX_SERIALIZABLE_H

#include "Interface.h"
#include "BinaryFileReader.h"
#include "BinaryFileWriter.h"

namespace px {

enum SerializeType {
	SERIALIZE_DEFAULT,
	SERIALIZE_RAW,
	SERIALIZE_COMPRESSED
};

class Serializable : public Interface {
public:
	virtual bool serialize(BinaryFileWriter& out, SerializeType type = SERIALIZE_DEFAULT) = 0;
	virtual bool deserialize(BinaryFileReader& in, SerializeType type = SERIALIZE_DEFAULT) = 0;
};

}

#endif
