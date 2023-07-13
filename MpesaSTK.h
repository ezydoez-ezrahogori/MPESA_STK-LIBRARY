
#ifndef MPESASTK_H
#define MPESASTK_H

#include <Arduino.h>


#define	SANDBOX		"https://sandbox.safaricom.co.ke/"
#define PRODUCTION	"https://api.safaricom.co.ke/"

#define	PAYBILL		"CustomerPayBillOnline"
#define	BUY_GOODS	"CustomerBuyGoodsOnline"


#define TEST_CODE	174379

class MpesaSTK {
	
	public:
		MpesaSTK(String consumer_key, String consumer_secret, String pass_key, String environment = SANDBOX);
		void begin(int code = TEST_CODE, String type = PAYBILL, String url = "http://mycallbackurl.com/checkout.php", int gmt = 3, bool set = true);
		String pay(String number, int amount, String reference, String description);
		
	private:	
		bool getToken();
		String encoder64(String input);
		String stkPush();
		void setTime(int gmt, int dst = 0);
		String getTime(String format);
		tm getTimeStruct();
		
		int _amount;
		int _business_code;
		String _consumer_key;
		String _consumer_secret;
		String _pass_key;
		String _callback_url;
		String _transaction_type;
		String _token;
		String _number;
		String _reference;
		String _description;
		String _environment;
};


#endif
