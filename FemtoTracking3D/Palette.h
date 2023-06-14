
#ifndef PX_PALETTE_H
#define PX_PALETTE_H

#include <memory>
#include <string>
#include <sstream>
#include <vector>

namespace px {

template <typename Real>
struct PaletteElement { Real r, g, b; };

template <typename Real>
class Palette {
public:
	Palette();
	Palette(std::size_t n);
	Palette(const Palette<Real>& p);
	Palette(const std::vector<PaletteElement<Real> >& values);
	virtual ~Palette();

	template <std::size_t N>
	bool load(const float (&data)[N]);
	bool load(const std::string& filename);
	bool save(const std::string& filename) const;

	bool get(Real p, PaletteElement<Real>& value);
	bool getValue(Real p, PaletteElement<Real>& value);
	bool getMinimumValue(PaletteElement<Real>& value);
	bool getMaximumValue(PaletteElement<Real>& value);

	bool copy(const Palette<Real>& p);
	bool gradient(std::size_t n, Real minimum, Real maximum);
	bool zero();
	bool reset();

	bool setName(const std::string& name);

	std::size_t size() const;
	
	std::string getName() const;
	std::string toString() const;
	PaletteElement<Real>* getData();
	const PaletteElement<Real>* constData() const;

	Palette<Real>& operator = (const Palette<Real>& p);

protected:
	void allocate(std::size_t n);
	std::string getName(const std::string& filename) const;

public:
	std::size_t n;
	PaletteElement<Real>* data;
	std::string name;
};

//------------------------------------------------------------------------------
// Implementation
//------------------------------------------------------------------------------

template <typename Real>
inline bool PaletteScale(Real& v, Real sourceMin, Real sourceMax, Real targetMin, Real targetMax) {
	if ( targetMax < targetMin ) return false;
	if ( sourceMax < sourceMin ) return false;
	if ( v > sourceMax ||  v < sourceMin ) return false;
	
	Real l = (targetMax - targetMin);
	Real m = l / (sourceMax - sourceMin);
    if ( l == static_cast<Real>(0) ) m = static_cast<Real>(0);
	v = targetMin + ((v-sourceMin) * m);
	return true;
}

template <typename Real>
Palette<Real>::Palette() {
	this->n = 0;
	this->data = nullptr;
}

template <typename Real>
Palette<Real>::Palette(std::size_t n) {
	this->allocate(n);
}

template <typename Real>
Palette<Real>::Palette(const Palette<Real>& p) {
	this->allocate(p.n);
	this->copy(p);
}

template <typename Real>
Palette<Real>::Palette(const std::vector<PaletteElement<Real> >& values) {
	this->allocate(values.size());
	for ( std::size_t i = 0; i < this->n; i++ )
		this->data[i].r = this->data[i].g = this->data[i].b = values[i];
}

template <typename Real>
Palette<Real>::~Palette() {
	delete [] this->data;
	this->data = nullptr;
	this->n = 0;
}

template <typename Real>
template <std::size_t N>
bool Palette<Real>::load(const float (&data)[N]) {
	if ( data == nullptr ) {
		std::cerr << "[Palette:Palette] Error: Nullptr data array." << std::endl;
		return false;
	}

	std::size_t ds = sizeof(data);
	std::size_t ts = sizeof(data[0]);
	this->n = ds / ts / 3;
	this->allocate(this->n);

	std::size_t index = 0;
	for ( std::size_t i = 0; i < this->n; i++ ) {
		this->data[i].r = data[index++];
		this->data[i].g = data[index++];
		this->data[i].b = data[index++];
	}

	return true;
}

template <typename Real>
bool Palette<Real>::load(const std::string& filename) {
	if ( filename.length() == 0 ) return false;
	this->clear();

	std::ifstream in;
	in.open(filename.c_str());

	if ( in.is_open() == false ) {
		std::cerr << "[Palette:load] Error: Could not load file: " << filename << std::endl;
		return false;
	}

	this->name = this->getName(filename);
	std::vector<Real> rValues, gValues, bValues;
	std::string line, token;
	while ( std::getline(in, line) ) {
		std::stringstream stream(line);
		std::getline(stream, token, ' ');

		if ( token.length() == 0 ) continue;
		if ( token[0] == '#' ) continue;

		rValues.push_back(std::atof(token.c_str()));
		std::getline(stream, token, ' ');
		gValues.push_back(std::atof(token.c_str()));
		std::getline(stream, token, ' ');
		bValues.push_back(std::atof(token.c_str()));
	}

	if ( rValues.size() != gValues.size() || gValues.size() != bValues.size() ) {
		std::cerr << "[Palette:load] Error: Invalid format, component counts do not match." << std::endl;
		in.close();
		return false;
	}

	std::size_t n = rValues.size();
	this->allocate(n);

	for ( std::size_t i = 0; i < this->n; i++ ) {
		this->data[i].r = static_cast<Real>(rValues[i]);
		this->data[i].g = static_cast<Real>(gValues[i]);
		this->data[i].b = static_cast<Real>(bValues[i]);
	}

	in.close();

	return true;
}

template <typename Real>
bool Palette<Real>::save(const std::string& filename) const {
	if ( filename.length() == 0 ) return false;

	std::ofstream out;
	out.open(filename.c_str());

	for ( std::size_t i = 0; i < this->n; i++ ) {
		out << this->data[i].r << ' ' << this->data[i].g << ' ' << this->data[i].b << std::endl;
	}

	out.close();
	return true;
}

template <typename Real>
bool Palette<Real>::get(Real p, PaletteElement<Real>& value) {
	return this->getValue(p, value);
}

template <typename Real>
inline bool Palette<Real>::getValue(Real p, PaletteElement<Real>& value) {
	//if ( this->n == 0 ) return false;
	//if ( p < Real(0) || p > Real(1) ) {
	//	std::cerr << "[Palette:getValue] Error: Percentage must be between 0 and 1." << std::endl;
	//	return false;
	//}

	if ( std::isfinite(p) == false || p < Real(0) || p > Real(1) ) {
		value.r = Real(1);
		value.g = Real(1);
		value.b = Real(1);
		return false;
	}
	
	value = this->data[int(p * (this->n - 1))];
	return true;
}

template <typename Real>
bool Palette<Real>::getMinimumValue(PaletteElement<Real>& value) {
	return this->getValue(Real(0), value);
}

template <typename Real>
bool Palette<Real>::getMaximumValue(PaletteElement<Real>& value) {
	return this->getValue(Real(1), value);
}

template <typename Real>
bool Palette<Real>::copy(const Palette<Real>& p) {
	if ( p.n == 0 ) return false;
	this->allocate(p.n);
	for ( std::size_t i = 0; i < this->n; i++ ) {
		this->data[i].r = p.data[i].r;
		this->data[i].g = p.data[i].g;
		this->data[i].b = p.data[i].b;
	}
	return true;
}

template <typename Real>
bool Palette<Real>::gradient(std::size_t n, Real minimum, Real maximum) {
	if ( maximum < minimum ) return false;
	if ( minimum == maximum ) return this->zero();
	this->allocate(n);

	Real value = Real(0);
	Real increment = std::abs((maximum - minimum) / static_cast<Real>(n - 1));
	PaletteScale(increment, Real(0), std::abs(maximum - minimum), Real(0), Real(1));

	for ( std::size_t i = 0; i < n; i++ ) {
		this->data[i].r = this->data[i].g = this->data[i].b = value;
		value += increment;
	}

	return true;
}

template <typename Real>
bool Palette<Real>::zero() {
	if ( this->n == 0 ) return false;
	std::memset(this->data, 0, sizeof(PaletteElement<Real>) * n);
	return true;
}

template <typename Real>
bool Palette<Real>::reset() {
	this->n = 0;
	delete [] this->data;
	this->data = nullptr;
	return true;
}

template <typename Real>
bool Palette<Real>::setName(const std::string& name) {
	if ( name.length() == 0 ) return false;
	this->name = name;
	return true;
}

template <typename Real>
std::size_t Palette<Real>::size() const {
	return this->n;
}

template <typename Real>
std::string Palette<Real>::getName() const {
	return this->name;
}

template <typename Real>
PaletteElement<Real>* Palette<Real>::getData() {
	return this->data;
}

template <typename Real>
const PaletteElement<Real>* Palette<Real>::constData() const {
	return this->data;
}

template <typename Real>
std::string Palette<Real>::toString() const {
	std::stringstream stream;
	stream << "[" << std::endl;
	for ( std::size_t i = 0; i < this->n; i++ ) {
		if ( i == n - 1 ) stream << "(" << this->data[i].r << " " << this->data[i].g << " " << this->data[i].b << ")" << std::endl;
		else stream << "(" << this->data[i].r << " " << this->data[i].g << " " << this->data[i].b << ")" << std::endl; 
	}
	stream << "]";
	return stream.str();
}

template <typename Real>
Palette<Real>& Palette<Real>::operator = (const Palette<Real>& p) {
	if ( this == &p ) return *this;
	this->allocate(p.n);
	for ( std::size_t i = 0; i < this->n; i++ ) {
		this->data[i].r = p.data[i].r;
		this->data[i].g = p.data[i].g;
		this->data[i].b = p.data[i].b;
	}
	return *this;
}

template <typename Real>
std::string Palette<Real>::getName(const std::string& filename) const {
	if ( filename.length() == 0 ) return "None";
	std::stringstream stream(filename);
	std::string name;
	std::getline(stream, name, '.');
	for ( std::size_t i = 0; i < name.length(); i++ )
		if ( name[i] == '\\' || name[i] == '/' ) name[i] = '_';
	return name;
}

template <typename Real>
void Palette<Real>::allocate(std::size_t n) {
	if ( this->data == nullptr ) this->data = new PaletteElement<Real>[n];
	else if ( this->data != nullptr && n != this->n ) {
		delete [] this->data;
		this->data = new PaletteElement<Real>[n];
	}

	this->n = n;
	std::memset(this->data, 0, sizeof(PaletteElement<Real>) * n);
}

}

#endif
