#ifndef CONFIG_H
#define CONFIG_H

#define DIR_TOWARDS()   GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET)
#define DIR_BACK()      GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);


#endif
