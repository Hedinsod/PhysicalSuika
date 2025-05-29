#pragma once


template <class EnumType>
class TEnum
{
public:
	TEnum(EnumType EnumValue)
		: Value(static_cast<uint8_t>(EnumValue))
	{ }

	// Getters
	EnumType ToEnum()
	{
		return static_cast<EnumType>(Value);
	}
	uint8_t ToInteger()
	{
		return Value;
	}

	// Comfort
	bool operator ==(TEnum<EnumType> OtherEnum)
	{
		return OtherEnum.Value == Value;
	}
	bool operator ==(EnumType OtherEnum)
	{
		return OtherEnum == ToEnum();
	}

	// TODO: Range checks?
	EnumType operator+(uint8_t Addition)
	{
		uint8_t NewValue = Value + Addition;
		return static_cast<EnumType>(NewValue);
	}
	EnumType operator++()
	{
		++Value;
		return ToEnum();
	}
	EnumType operator++(int)
	{
		EnumType Temp = ToEnum();
		++Value;
		return Temp;
	}

	EnumType operator-(uint8_t Deduction)
	{
		uint8_t NewValue = Value - Deduction;
		return static_cast<EnumType>(NewValue);
	}
	EnumType operator--()
	{
		--Value;
		return ToEnum();
	}
	EnumType operator--(int)
	{
		EnumType Temp = ToEnum();
		--Value;
		return Temp;
	}

private:
	uint8_t Value;

};
