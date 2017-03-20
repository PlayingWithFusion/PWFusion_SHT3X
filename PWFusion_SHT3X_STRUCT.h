#ifndef PWF_SHT3X_STRUCT_H
#define PWF_SHT3X_STRUCT_H

struct var_SHT3X{
	uint16_t  temp_raw;			// temperature. *175/65535 - 45 = RWV
	uint16_t  relhum_raw;		// relative humidity. *100/65535 = RWV
};

#endif /* PWF_SHT3X_STRUCT_H */
