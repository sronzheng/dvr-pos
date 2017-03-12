
#ifndef __POS_STRUCT_H__
#define __POS_STRUCT_H__

//
// POS.dll output format
//

/*
* Sale item data
*/
typedef struct _SALE_ITEM_DATA
{
	char	name[36];		// sale item name
	int		unitPrice;		// unit price of sale item
	int		quantity;		// quantity of sale item
	int		price;			// total price of sale item

} SALE_ITEM_DATA;

/*
* Output data
*/
typedef struct _POS_DATA
{
	char keyword[24];				// keyword of output	
	union
	{
		UINT64			dateTime;	// for "Transaction"
		SALE_ITEM_DATA	itemData;	// for "Item"
		int				amount;		// for other items
	};

} POS_DATA;

#endif