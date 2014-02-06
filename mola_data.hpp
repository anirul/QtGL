
namespace MOLA {

    class data_interface {
    public :
	virtual size_t size() const = 0;
	virtual uint32_t get_image_pitch() const = 0;
	virtual const float* get_image_pointer() const = 0;
	virtual std::vector<float> get_image_window(
	    const std::pair<uint32_t, uint32_t>& pos,
	    const std::pair<uint32_t, uint32_t>& win) const = 0;
	virtual std::ostream& operator<<(std::ostream& os) const = 0;
    };

    template <class T>
    class data : public data_interface {
    protected :
	uint32_t pitch_;
	std::vector<T> data_;
	std::vector<float> image_;
	T min_;
	T max_;
    public :
	T operator()(uint32_t x, uint32_t y) const {
	    assert((x + y * pitch_) < data_.size());
	    return data_[x + y * pitch_];
	}
	data(const std::string& in_file, uint32_t pitch) {
	    pitch_ = pitch;
	    min_ = std::numeric_limits<T>::max();
	    max_ = std::numeric_limits<T>::min();
	    FILE* file = fopen(in_file.c_str(), "rb");
	    if (!file) 
		throw std::runtime_error(
		    std::string("error openning file ") + in_file);
	    size_t ret;
	    do {
		T elem;
		switch (sizeof(T)) {
		case 1:
		{
		    uint8_t buffer;
		    ret = fread(&buffer, sizeof(T), 1, file);
		    memcpy(&elem, &buffer, sizeof(T));
		}
		    break;
		case 2:
		{
		    uint16_t buffer;
		    ret = fread(&buffer, sizeof(T), 1, file);
		    uint16_t value = ntohs(buffer);
		    memcpy(&elem, &value, sizeof(T));
		}
		    break;
		case 4:
		{
		    uint32_t buffer;
		    ret = fread(&buffer, sizeof(T), 1, file);
		    uint32_t value = ntohl(buffer);
		    memcpy(&elem, &value, sizeof(T));
		}
		    break;
		default:
		    throw std::runtime_error("unknown type");
		}
		if (ret != 0) {
		    if (elem < min_) min_ = elem;
		    if (elem > max_) max_ = elem;
		    data_.push_back(elem);
		}
	    } while (ret != 0);
	    std::cout << "total length    : " << data_.size() << std::endl;
	    fclose(file);
	    { // create the image vector
		image_.resize(data_.size());
		T delta = max_ - min_;
		for (uint32_t i = 0; i < pitch_; ++i) {
		    for (uint32_t j = 0; j < (data_.size() / pitch_); ++j) {
			image_[(pitch_ * j) + i] = 
			    (float)(this->operator()(i, j) - min_) / (float)delta;
		    }
		}
	    }
	}
	virtual size_t size() const {
	    return data_.size();
	}
	virtual uint32_t get_image_pitch() const {
	    return pitch_;
	}
	virtual const float* get_image_pointer() const {
	    return &image_[0];
	}
	virtual std::ostream& operator<<(std::ostream& os) const {
	    T delta = max_ - min_;
	    for (uint32_t i = 0; i < pitch_; ++i) {
		for (uint32_t j = 0; j < (data_.size() / pitch_); ++j) {
		    os << (float)(this->operator()(i, j) - min_) / (float)delta;
		    os << ",";
		}
		os << std::endl;
	    }
	    return os;
	}
	virtual std::vector<float> get_image_window(
	    const std::pair<uint32_t, uint32_t>& pos,
	    const std::pair<uint32_t, uint32_t>& win) const {
	    assert(pos.first < win.first);
	    assert(pos.second < win.second);
	    assert(win.first <= pitch_);
	    assert(win.second <= (image_.size() / pitch_));
	    std::vector<float> out;
	    for (uint32_t i = pos.first; i < pos.first + win.first; ++i) {
		for (uint32_t j = pos.second; j < pos.second + win.second; ++j) {
		    out.push_back(image_[(pitch_ * j) + i]); 
		}
	    }
	    return out;
	}
    };

}
