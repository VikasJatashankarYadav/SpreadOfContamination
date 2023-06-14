
#ifndef PX_DEPTH_CLOUD_H
#define PX_DEPTH_CLOUD_H

#include "OrganizedCloud.h"

namespace px {

class DepthCloud : public OrganizedCloud<PointXYZ<Real> > {
public:
	DepthCloud();
	DepthCloud(std::size_t width, std::size_t height);
	virtual ~DepthCloud();

	/* Serialize interface. This implementation compresses the
	 * data prior to being written. This reduces the accuracy
	 * of the data at each write, therefore should be used sparingly.
	 */
	bool serialize(BinaryFileWriter& out, SerializeType type = SERIALIZE_COMPRESSED);
	bool deserialize(BinaryFileReader& in, SerializeType type = SERIALIZE_COMPRESSED);

	bool scale(Real uniform_scale);

	/* Min and max distance in the data (z), (used for compression).
	 * The distance bounds should be set to  the min and max distance values (typically the z component)
	 * of the point data in the cloud. The closer these values are to the expected distances
	 * (hardware dependent), the compression accuracy will improve.
	 */
	bool setDistanceBounds(Real min, Real max);

	/* Min and max (x,y) real distances (left, right extents), used for compression.
	 * The minimum and maximum values are used for both (x) and (y) components
	 * of the internal point data.
	 */
	bool setRangeBounds(Real min, Real max);

	Real getMinDepth() const;
	Real getMaxDepth() const;

	/* Min/Max Distance is the manually set distance used for compression. 
	 * These values are not related to the depth point distances.
	 */
	Real getMinDistance() const;
	Real getMaxDistance() const;
	Real getMinRange() const;
	Real getMaxRange() const;

	static bool Load(const std::string& filename, std::vector<std::shared_ptr<DepthCloud> >& clouds);

protected:
	/* Distance and range values correspond to the global coordinates of
	 * the points in the depth point values. Distance corresponds to
	 * the (z) value and the range min/max corresponds to the (x) and (y)
	 * values. These values depend on the device/hardware generating the
	 * depth image. For short range devices this may be from 0.1m to 1.5m.
	 * For mid-range devices this may be from 0.5m to 8.0m. For long-range
	 * devices it may be from 1.0m to 20m or more.
	 */
	Real min_distance, max_distance;
	Real min_range, max_range;

private:
	/* Stores compressedd data for rapid-write to storage. The compression
	 * accuracy depends on the min/max distance and min/max range values.
	 * The closer these values are to the depth data values stored inside
	 * the cloud, the compression accuracy will improve.
	 */
	std::vector<int16_t> compressed;
};

}

#endif
