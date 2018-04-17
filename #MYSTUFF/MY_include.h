#define F_CPU 16e6

//DEFINE bit manipulation:
#define setbit(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define clearbit(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define flipbit(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))
#define checkbit(ADDRESS,BIT) (ADDRESS & (1<<BIT))
#define setbitmask(x,y) (x |= (y))
#define clearbitmask(x,y) (x &= (~y))
#define flipbitmask(x,y) (x ^= (y))
#define checkbitmask(x,y) (x & (y))