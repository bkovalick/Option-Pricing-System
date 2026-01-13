#ifndef PricingEngine_HPP
#define PricingEngine_HPP


class PricingEngine
{
public:
	virtual ~PricingEngine() = default;
	virtual double calculatePrice() const = 0;
};

#endif