#pragma once

class CBase64Manager
{
public:

	uint32_t base64_encode(const unsigned char *text, uint32_t text_len, unsigned char *encode);
	uint32_t base64_decode(const unsigned char *code, uint32_t code_len, unsigned char *plain);

private:

};

