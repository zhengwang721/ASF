
#include "at_ble_api.h"

#include "platform.h"
#include "gattc_task.h"
#include "gattm_task.h"
#include "gapm_task.h"
#include "gapc_task.h"
#include "dbg_task.h"

#include "device.h"
#include "event.h"



// Slave preferred Connection interval Min
#define APP_CON_INTV_MIN	8
// Slave preferred Connection interval Max
#define APP_CON_INTV_MAX	10
// Slave preferred Connection latency
#define APP_CON_LATENCY		0
// Slave preferred Link supervision timeout
#define APP_SUPERV_TO		200 // 2s (500*10ms)

#define APP_MAX_MTU			L2C_MIN_LE_MTUSIG
#define APP_FLAGS			0

#define LOCAL_ADDR_TYPE GAPM_PUBLIC_ADDR
#define GAP_device_name "ATMEL_BLE"

struct device_info device;
/*
void prepare_apis(wr_apis* plf_wr_apis)
{
	plf_wr_apis->wr_api32 = dbg_wr_mem_req_handler32;
	plf_wr_apis->wr_api32_reset = dbg_wr_mem_req_handler32_reset;
}*/
#ifndef __WINC3000__
const uint32_t fw_patch[] = {
0xB4FFB500, 0x46494640, 0x465B4652, 0xF3EF4664, 0xB43F8503, 
0x8009F3EF, 0x681A4B04, 0x98066010, 0x9A089907, 0x9C0A9B09, 
0x47709D0B, 0x10000970, 0x49484849, 0x21006141, 0x21056181, 
0x47706101, 0x69804845, 0x4D454770, 0x4F464C45, 0x68204E46, 
0xD4790400, 0x28017830, 0xBF30D001, 0x2000E7F7, 0xF7FF60A8, 
0x68A8FFCD, 0x68A82800, 0x2801D02F, 0x493ED1ED, 0x61080380, 
0xFBE4F001, 0xFFD8F7FF, 0x6800483B, 0x22014780, 0x06D0493A, 
0x483A6008, 0xB6626182, 0xF0012000, 0x2001FBDB, 0xFBD8F001, 
0xF0012002, 0x2003FBD5, 0xFBD2F001, 0xF0012004, 0x2005FBCF, 
0xFBCCF001, 0x69814830, 0xD4FC00C9, 0x6079492F, 0x07C96941, 
0x492ED021, 0x1C40E021, 0x200F60A8, 0xFBC2F001, 0xF001482B, 
0x481EFBC5, 0x492A6980, 0x1A086809, 0x492903C0, 0xFBC2F001, 
0x680A4928, 0x60081A10, 0x380F6808, 0xFBC0F001, 0xFBC4F001, 
0x68014824, 0xD5FC0449, 0x491DE7FE, 0x60B93111, 0x07496941, 
0x4920D40E, 0x210162B9, 0x61210289, 0x01C06980, 0xD5016820, 
0xE0004308, 0x60204388, 0xE007E002, 0x60F84819, 0xFBACF001, 
0x28017830, 0xE782D0FC, 0xE780BF30, 0x48154916, 0xE76D6048, 
0x00FFFFFF, 0xE000E000, 0x100000D0, 0x40011000, 0x4000B040, 
0x100008C8, 0x40010000, 0x100008E0, 0xE000E100, 0x40004000, 
0x4000B140, 0x22222222, 0x20222222, 0x002700FF, 0x100098D4, 
0x018CBA80, 0x10000914, 0x4000D000, 0x00004231, 0x00002222, 
0x12345679, 0x4000F040, 0x2101B510, 0x07094802, 0xF0016048, 
0xBD10FB75, 0x00018119, 0xB5104770, 0xFF3CF7FF, 0x600849FE, 
0x6B0849FE, 0x43102207, 0xBD106308, 0x4AFCB5F0, 0x6851B08B, 
0x43191593, 0x4CFA6051, 0x68252100, 0x006D086D, 0x4CF86025, 
0x4CF86244, 0x4CF86284, 0x25066144, 0x54252430, 0x62C44CF6, 
0x60044CF6, 0x62044CF6, 0x60444CF6, 0x4CF64FEC, 0x61C43780, 
0x247F6838, 0x43A00624, 0x4320066C, 0x68386038, 0x042424FF, 
0x242143A0, 0x43200464, 0x68386038, 0x0224240F, 0x240743A0, 
0x43200224, 0x683C6038, 0x0A242056, 0x43040224, 0x466C603C, 
0x54600048, 0xB2C91C49, 0xD3F92928, 0x69E04CE3, 0x43082102, 
0x48E261E0, 0x60C4240D, 0x684448E1, 0x432C2503, 0x68C46044, 
0x60C4430C, 0x43186850, 0x48DD6050, 0x25806801, 0x60014329, 
0x06096801, 0x6901D4FC, 0x03F62601, 0x43B16854, 0x6054439C, 
0x25806804, 0x6004432C, 0x06246804, 0x6904D4FC, 0x03F62601, 
0x610443B4, 0x68544AC2, 0x439C1593, 0x24E16054, 0x02646905, 
0x48CC43B5, 0xFACEF001, 0x436E4606, 0x46304621, 0xFAC8F001, 
0x4620B281, 0x1A324348, 0x09232000, 0x1C40E002, 0x1AD2B2C0, 
0xD3FA4293, 0x08421C40, 0x230048C1, 0x23036203, 0x00C96283, 
0x62414311, 0x48BE4BBF, 0x48BF6018, 0x49BF6058, 0x4ABF6099, 
0x4ABF60DA, 0x4CBF611A, 0x6198615C, 0x4CBB61D9, 0x625A621C, 
0x629C4CBC, 0x631962D8, 0x1E644CB7, 0x639A635C, 0x63DC4CB9, 
0x33404BB1, 0x60596018, 0x4CAF4BB2, 0x34401E5B, 0x60E260A3, 
0x61234BB4, 0x61A16160, 0x1E9B4BAD, 0x622261E3, 0x62634BB1, 
0x62E162A0, 0x1E9B4BA9, 0x63626323, 0x63A34BAE, 0x4BA363E0, 
0x60193380, 0x4CA14BA4, 0x34801E9B, 0x60A26063, 0x60E34BA9, 
0x61616120, 0x1EDB4B9F, 0x61E261A3, 0x051B235D, 0x62606223, 
0x4B9B62A1, 0x62E31EDB, 0x4BA26322, 0x63A06363, 0x4B9763E1, 
0x1F1B4C93, 0x602334C0, 0x4B9E6062, 0x60E060A3, 0x4C926121, 
0x1F244B8E, 0x615C33C0, 0x4C8C619A, 0x34C04B99, 0x622061E3, 
0x4B8C6261, 0x62A31F1B, 0x4B9662E2, 0x63606323, 0x4B8863A1, 
0x63E31F5B, 0x601A4B93, 0x4B934C92, 0x60A06063, 0x4B8360E1, 
0x61231F5B, 0x4B906162, 0x61E061A3, 0x4B7F6221, 0x62631F5B, 
0x4B8D62A2, 0x632062E3, 0x63594623, 0x1F9B4B7A, 0x63E263A3, 
0x4B894C85, 0x60233440, 0x60A16060, 0x1F9B4B75, 0x612260E3, 
0x61634B85, 0x61E161A0, 0x1FDB4B71, 0x62626223, 0x62A34B82, 
0x462362E0, 0x4B6D6319, 0x63631FDB, 0x4B7F63A2, 0x4B7763E3, 
0x60183380, 0x4B686059, 0x1FDB4C74, 0x60A33480, 0x4B7A60E2, 
0x61606123, 0x4C6361A1, 0x3C084B6F, 0x61DC3380, 0x232F621A, 
0x055B4D6C, 0x626B3580, 0x62E962A8, 0x636A632C, 0x63AB4B71, 
0x4B6763E8, 0x601933C0, 0x609A605C, 0x60DC4C6E, 0x61596118, 
0x3C094C55, 0x61DA619C, 0x621D4D6B, 0x62996258, 0x631A62DC, 
0x635C4C69, 0x63D96398, 0x4B684C4E, 0x601C3C0A, 0x4D67605A, 
0x60D8609D, 0x615C6119, 0x4D65619A, 0x621861DD, 0x629C6259, 
0x4C6362DA, 0x6358631C, 0x4C446399, 0x63DC3C0B, 0x33404B5C, 
0x4D5F601A, 0x6098605D, 0x611C60D9, 0x4D5D615A, 0x61D8619D, 
0x625C6219, 0x4C5B629A, 0x631862DC, 0x4C396359, 0x639C3C0C, 
0x4B5163DA, 0x33804D57, 0x6058601D, 0x60DC6099, 0x4855611A, 
0x48306158, 0x61D96198, 0x4611621C, 0x215F6259, 0x62990509, 
0x492C62D8, 0x1E616319, 0x46116359, 0x494D6399, 0x494363D9, 
0x600831C0, 0x49264A41, 0x605132C0, 0x60911E61, 0x60D14925, 
0x61114947, 0x49216150, 0x1EA16191, 0x492161D1, 0x49446211, 
0x46116251, 0x491C6288, 0x493662D1, 0x31C01EA2, 0x4A34630A, 
0x32C0491A, 0x49326351, 0x31C04A3D, 0xE079638A, 0x100098D4, 
0x40040000, 0x4000F400, 0x40041800, 0x00017407, 0x000173F9, 
0x0001716F, 0x100081C3, 0x00017141, 0x0001715B, 0x0001715D, 
0x100081C1, 0x40020000, 0x40020840, 0x4000F000, 0x4000C000, 
0x018CBA80, 0x40004000, 0x05C62762, 0x40022800, 0x0F141A07, 
0x6A0391EF, 0x0000D3A0, 0x0F600113, 0x05C76276, 0x05C89D8A, 
0x05C9D89E, 0x05CB13B1, 0x05CC4EC5, 0x05CD89D9, 0x05CEC4EC, 
0x05D13B14, 0x05D27627, 0x05D3B13B, 0x05D4EC4F, 0x40022900, 
0x05D62762, 0x05D76276, 0x05D89D8A, 0x05D9D89E, 0x05DB13B1, 
0x05DC4EC5, 0x05DD89D9, 0x05DEC4EC, 0x05E13B14, 0x05E27627, 
0x05E3B13B, 0x05E4EC4F, 0x40022A00, 0x05E62762, 0x05E76276, 
0x05E89D8A, 0x05E9D89E, 0x05EB13B1, 0x05EC4EC5, 0x05ED89D9, 
0x05EEC4EC, 0x05F13B14, 0x05F27627, 0x05F3B13B, 0x05F4EC4F, 
0x4AFF63C8, 0x601149FD, 0x60511EA1, 0x609149FD, 0x60D149FD, 
0x49F96110, 0x1EE16151, 0x49F96191, 0x4AFB61D1, 0x601149F9, 
0x4CF46050, 0x60943C18, 0x60D149F8, 0x4AF84BF6, 0x23B9611A, 
0x04DB4DF4, 0x61A8616B, 0x622961EC, 0x4BF4626A, 0x62E862AB, 
0x632B4623, 0x63AA6369, 0x63EB4BF1, 0x33404BEC, 0x46256018, 
0x6099605D, 0x4DEE60DA, 0x6158611D, 0x619D4625, 0x621A61D9, 
0x625D4DEB, 0x46256298, 0x631962DD, 0x4DE9635A, 0x63D8639D, 
0x46254BE0, 0x601D3380, 0x609A6059, 0x60DD4DE5, 0x46256118, 
0x6199615D, 0x4DE361DA, 0x6258621D, 0x629D4625, 0x631A62D9, 
0x635D4DE0, 0x46256398, 0x4BD463DD, 0x601933C0, 0x4DDD605A, 
0x60D8609D, 0x611D4625, 0x619A6159, 0x61DD4DDA, 0x46256218, 
0x6299625D, 0x4DD862DA, 0x6358631D, 0x639D4625, 0x4BD663D9, 
0x4DD6601A, 0x6098605D, 0x60DD4625, 0x615A6119, 0x04ED25BB, 
0x61D8619D, 0x621D4625, 0x629A6259, 0x62DD4DCF, 0x46256318, 
0x6399635D, 0x4BCA63DA, 0x33404DCC, 0x6058601D, 0x609D4625, 
0x611A60D9, 0x615D4DC9, 0x46256198, 0x621961DD, 0x4DC7625A, 
0x62D8629D, 0x631D4625, 0x639A6359, 0x63DD4DC4, 0x33804BBD, 
0x46256018, 0x6099605D, 0x4DC160DA, 0x6158611D, 0x619D4625, 
0x621A61D9, 0x625D4DBE, 0x46256298, 0x631962DD, 0x4DBC635A, 
0x63D8639D, 0x46254BB1, 0x601D33C0, 0x609A6059, 0x60DD4DB8, 
0x46256118, 0x6199615D, 0x4DB661DA, 0x6258621D, 0x629D4625, 
0x631A62D9, 0x635D4DB3, 0x46256398, 0x4BB263DD, 0x605A6019, 
0x609D4DB1, 0x462060D8, 0x61596118, 0x20BD619A, 0x61D804C0, 
0x621848AD, 0x6299625C, 0x48AC62DA, 0x63084619, 0x634848A9, 
0x63884620, 0x63C8488B, 0x461049A4, 0x60083140, 0x604848A6, 
0x608848A3, 0x60C84620, 0x61084885, 0x61484610, 0x618848A2, 
0x61C8489E, 0x62084620, 0x62484880, 0x62884610, 0x62C8489E, 
0x63084899, 0x63484620, 0x6388487B, 0x63C84610, 0x489A4993, 
0x60083180, 0x60484893, 0x60884620, 0x60C84875, 0x61084610, 
0x61484895, 0x6188488E, 0x61C84620, 0x62084870, 0x62484610, 
0x62884891, 0x62C84889, 0x63084620, 0x6348486B, 0x63884610, 
0x63C8488D, 0x48844982, 0x600831C0, 0x60484620, 0x60884865, 
0x60C84610, 0x61084888, 0x6148487E, 0x61884620, 0x61C84860, 
0x62084610, 0x62484884, 0x62884879, 0x62C84620, 0x6308485B, 
0x63484610, 0x63884880, 0x63C84874, 0x4620497F, 0x48566008, 
0x46106048, 0x487D6088, 0x486F60C8, 0x46206108, 0x48516148, 
0x46106188, 0x200361C8, 0x04004978, 0x487862C8, 0x210269C0, 
0x49764388, 0x206161C8, 0x61C801C0, 0x48744975, 0x48756208, 
0x48756248, 0x48756288, 0x487562C8, 0x48756308, 0x48756348, 
0x48756388, 0x496D63C8, 0x31404874, 0x48746008, 0x49756048, 
0x60484873, 0x48744966, 0x48756148, 0x60014973, 0x48742155, 
0x61010209, 0x48734961, 0x49716108, 0x61484872, 0x496B20FF, 
0x31403041, 0x48706048, 0x20806038, 0x496F6088, 0x49666108, 
0x6108486E, 0x496C20FF, 0x314030A1, 0x486D6388, 0x6001496B, 
0x20004960, 0x60C839C0, 0x495D6108, 0x31CF4868, 0x60013080, 
0x48674966, 0x60483180, 0x60884866, 0x49664863, 0x60C13080, 
0x6880485A, 0x03C92101, 0x49584308, 0x49536088, 0x39C02001, 
0x20FF6008, 0x608830F9, 0x4A5E494E, 0x61111C89, 0x495C460A, 
0x232960CA, 0x02DB494B, 0x604B39C0, 0x4C594D5A, 0x495A602C, 
0x608A2200, 0x36404E58, 0x22CF6032, 0x02124F48, 0x4A56613A, 
0x2709617A, 0x02BF4A45, 0x61173240, 0x4A464F53, 0xE0A5603A, 
0x6A0391EF, 0x40022B00, 0x0F600113, 0x05F62762, 0x05C6C4EC, 
0x40022C00, 0x0000C890, 0x03600513, 0x05C93B14, 0x05CA7627, 
0x05CBB13B, 0x05CCEC4F, 0x05CE2762, 0x05CF6276, 0x05D09D8A, 
0x05D1D89E, 0x05D313B1, 0x05D44EC5, 0x05D589D9, 0x40022D00, 
0x05D6C4EC, 0x05D93B14, 0x05DA7627, 0x05DBB13B, 0x05DCEC4F, 
0x05DE2762, 0x05DF6276, 0x05E09D8A, 0x05E1D89E, 0x05E313B1, 
0x05E44EC5, 0x05E589D9, 0x40022E00, 0x05E6C4EC, 0x0F141A07, 
0x05E93B14, 0x05EA7627, 0x05EBB13B, 0x05ECEC4F, 0x05EE2762, 
0x05EF6276, 0x05F09D8A, 0x05F1D89E, 0x05F313B1, 0x05F44EC5, 
0x05F589D9, 0x40022F00, 0x05F6C4EC, 0x40041A80, 0x40020000, 
0x03020100, 0x40030000, 0x07060504, 0x0B0A0908, 0x0F0E0D0C, 
0x13121110, 0x17161514, 0x1B1A1918, 0x1F1E1D1C, 0x23222120, 
0x27262524, 0x00010001, 0x400250C0, 0x6A0091FF, 0x7F60010F, 
0x4000F400, 0x40020800, 0x0F141607, 0x0010108E, 0x08600140, 
0x40025240, 0x00020020, 0xC30041A0, 0x40041800, 0x00000A1E, 
0x1E1E0101, 0x00001111, 0x40024A00, 0x000A1FFF, 0x40020180, 
0x40024000, 0x0014148E, 0x40040080, 0x4AFD4FFE, 0x4FFD603A, 
0x37804AFD, 0x4AFD603A, 0x4AFD607A, 0x4AFD60BA, 0x4FFE60FA, 
0x607A4AFC, 0x603A4AFD, 0x60BA4AFD, 0x37404FFA, 0x4FFC60BA, 
0x603A2201, 0x4FFB4AF2, 0x613A3ACD, 0x4FF860FA, 0x37804AF9, 
0x62FA63FA, 0x62BA627A, 0x321A4AF6, 0x4FF7637A, 0x62BA4AF5, 
0x4AF64FF1, 0x613A37C0, 0x4FF522FF, 0x607A3281, 0x60BA2208, 
0x4FF022FF, 0x3740326D, 0x2210603A, 0x4FE9613A, 0x607B60B8, 
0x2000602C, 0x60306088, 0x4BEC48ED, 0x4BED6143, 0x4BED6183, 
0x4BED61C3, 0x49E4618B, 0x620131F3, 0x62412100, 0x200149D5, 
0x61483180, 0x212D48E8, 0x4CE86081, 0x04956B20, 0x432843A8, 
0x6B206320, 0x438803D1, 0x63204308, 0x49E22021, 0x31400140, 
0x6B206048, 0x03092103, 0x63204388, 0x01516B20, 0x63204388, 
0x10496B20, 0x63204388, 0x21806B20, 0x43084388, 0x6B206320, 
0x43882160, 0x6B206320, 0x63204390, 0x08406B20, 0x63200040, 
0x6B6049BD, 0x0C0031D4, 0x43080400, 0x6B606360, 0x0409211F, 
0x03514388, 0x63604308, 0x010E6B20, 0x433043B0, 0xF0006320, 
0xF000FDBD, 0xF000FDC1, 0x6B20FDC5, 0x632043B0, 0x11296B20, 
0x43084388, 0x6B206320, 0x632043A8, 0x680048C0, 0x48C04780, 
0x60812100, 0xBDF0B00B, 0x49BEB5F8, 0x61C82008, 0x680048A3, 
0x084049A2, 0x60080040, 0x304048B6, 0x227F6801, 0x43910612, 
0x06922203, 0x60014311, 0x22FF6801, 0x43910412, 0x04522221, 
0x60014311, 0x220F6801, 0x43910212, 0x02122207, 0x60014311, 
0x21566802, 0x02120A12, 0x6002430A, 0x69C048A5, 0x49A42602, 
0x61C84330, 0x210D4893, 0x60C13040, 0x684148A3, 0x43112203, 
0x68C16041, 0x60C14331, 0x48A14BA2, 0x4AA26018, 0x49A2605A, 
0x48A26099, 0x48A260D8, 0x4CA26118, 0x619A615C, 0x4C9E61D9, 
0x6258621C, 0x629C4C9F, 0x631962DA, 0x1E644C9A, 0x6398635C, 
0x63DC4C9C, 0x33404B94, 0x6059601A, 0x4C924B95, 0x34401E5B, 
0x60E060A3, 0x61234B97, 0x61A16162, 0x1E9B4B90, 0x622061E3, 
0x62634B94, 0x62E162A2, 0x1E9B4B8C, 0x63606323, 0x63A34B91, 
0x4B8663E2, 0x60193380, 0x4C844B87, 0x34801E9B, 0x60A06063, 
0x60E34B8C, 0x61616122, 0x1EDB4B82, 0x61E061A3, 0x051B235D, 
0x62626223, 0x4B7E62A1, 0x62E31EDB, 0x4B856320, 0x63A26363, 
0x4B7A63E1, 0x1F1B4C76, 0x602334C0, 0x4B816060, 0x60E260A3, 
0x4B756121, 0x61631F1B, 0x4B7E61A0, 0x622261E3, 0x4B716261, 
0x62A31F1B, 0x4B7B62E0, 0x63626323, 0x4B6D63A1, 0x63E31F5B, 
0x60184B78, 0x4B784C77, 0x60A26063, 0x4B6860E1, 0x61231F5B, 
0x4B756160, 0x61E261A3, 0x4B646221, 0x62631F5B, 0x4B7262A0, 
0x632262E3, 0x63594623, 0x1F9B4B5F, 0x63E063A3, 0x4B6E4C6A, 
0x60233440, 0x60A16062, 0x1F9B4B5A, 0x612060E3, 0x61634B6A, 
0x61E161A2, 0x1FDB4B56, 0x62606223, 0x62A34B67, 0x462362E2, 
0x4B526319, 0x63631FDB, 0x4B6463A0, 0x4B5C63E3, 0x601A3380, 
0x4B4D6059, 0x1FDB4C59, 0x60A33480, 0x4B5F60E0, 0x61626123, 
0x4B4861A1, 0x61E33B08, 0x242F6220, 0x05644D52, 0x626C3580, 
0x62E962AA, 0x6368632B, 0x63AC4C57, 0x4C4D63EA, 0x602134C0, 
0x60A06063, 0x60E34B54, 0x61616122, 0x3B094B3B, 0x61E061A3, 
0x4C514D46, 0x622C35C0, 0x62A9626A, 0x632862EB, 0x636B4B4E, 
0x63E963AA, 0x4B4D4C33, 0x601C3C0A, 0x4D4C6058, 0x60DA609D, 
0x615C6119, 0x4D4A6198, 0x621A61DD, 0x629C6259, 0x4C4862D8, 
0x635A631C, 0x4C296399, 0x63DC3C0B, 0x33404B41, 0x4D446018, 
0x609A605D, 0x611C60D9, 0x49426158, 0x61DA6199, 0x62194920, 
0x6298625C, 0xE07F493F, 0xC30041A0, 0x40041800, 0x000100D0, 
0x00000A1E, 0x1E1E0101, 0x00001111, 0x8228EE03, 0x40020800, 
0x00002252, 0x00000EEA, 0x40025000, 0x40024A00, 0x00010208, 
0x0000022F, 0x40025200, 0x00020002, 0x40025100, 0xCDDCF0FF, 
0x40041900, 0x849DB9CD, 0x0000567E, 0x00001D5A, 0x40020000, 
0x40040040, 0x100008F8, 0x4000F000, 0x40024000, 0x05C62762, 
0x40022800, 0x0F141A07, 0x6A0391EF, 0x0000D3A0, 0x0F600113, 
0x05C76276, 0x05C89D8A, 0x05C9D89E, 0x05CB13B1, 0x05CC4EC5, 
0x05CD89D9, 0x05CEC4EC, 0x05D13B14, 0x05D27627, 0x05D3B13B, 
0x05D4EC4F, 0x40022900, 0x05D62762, 0x05D76276, 0x05D89D8A, 
0x05D9D89E, 0x05DB13B1, 0x05DC4EC5, 0x05DD89D9, 0x05DEC4EC, 
0x05E13B14, 0x05E27627, 0x05E3B13B, 0x05E4EC4F, 0x40022A00, 
0x05E62762, 0x05E76276, 0x05E89D8A, 0x05E9D89E, 0x05EB13B1, 
0x05EC4EC5, 0x631A62D9, 0x635949FE, 0x63991E61, 0x4BFE63D8, 
0x601949FC, 0x49FA605A, 0x1E616099, 0x611860D9, 0x615949FA, 
0x49F6619A, 0x1E6161D9, 0x62586219, 0x0509215F, 0x62DA6299, 
0x631949F1, 0x63591EA1, 0x49F36398, 0x49F063D9, 0x600A3140, 
0x49EC4BEE, 0x60593340, 0x60991EA1, 0x49EE60D8, 0x615A6119, 
0x619949E7, 0x61D91EE1, 0x49EB6218, 0x629A6259, 0x62D949E3, 
0x63191EE1, 0x49E86358, 0x63DA6399, 0x49DF4BE1, 0x60193380, 
0x60591EE1, 0x49E46098, 0x611A60D9, 0x615949DA, 0x61991F21, 
0x4BE261D8, 0x601848E0, 0x48D6605A, 0x60983818, 0x60D949DF, 
0x611848DF, 0x04E424B9, 0x619A615C, 0x3C184CD0, 0x621961DC, 
0x4CDB6258, 0x62DA629C, 0x3C184CCC, 0x6359631C, 0x4CD86398, 
0x4BD363DC, 0x601A3340, 0x3C184CC7, 0x6099605C, 0x4CD460D8, 
0x615A611C, 0x3C184CC3, 0x61D9619C, 0x4CD16218, 0x629A625C, 
0x3C184CBF, 0x631962DC, 0x4CCE6358, 0x63DA639C, 0x4BC54CBB, 
0x33803C18, 0x6059601C, 0x4CCA6098, 0x611A60DC, 0x3C184CB6, 
0x6199615C, 0x4CC761D8, 0x625A621C, 0x3C184CB2, 0x62D9629C, 
0x4CC46318, 0x639A635C, 0x3C184CAE, 0x4BB763DC, 0x601933C0, 
0x4CC06058, 0x60DA609C, 0x3C184CA9, 0x6159611C, 0x4CBD6198, 
0x621A61DC, 0x3C184CA5, 0x6299625C, 0x4CBA62D8, 0x635A631C, 
0x3C184CA1, 0x63D9639C, 0x60184BB7, 0x605C4CB7, 0x4C9D609A, 
0x60DC3C18, 0x61586119, 0x04E424BB, 0x61DA619C, 0x3C184C98, 
0x6259621C, 0x4CB06298, 0x631A62DC, 0x3C184C94, 0x6399635C, 
0x4BAA63D8, 0x33404CAC, 0x605A601C, 0x3C184C8F, 0x60D9609C, 
0x4CA96118, 0x619A615C, 0x3C184C8B, 0x621961DC, 0x4CA66258, 
0x62DA629C, 0x3C184C87, 0x6359631C, 0x4CA36398, 0x4B9C63DC, 
0x601A3380, 0x3C184C82, 0x6099605C, 0x4C9F60D8, 0x615A611C, 
0x3C184C7E, 0x61D9619C, 0x4C9C6218, 0x629A625C, 0x3C184C7A, 
0x631962DC, 0x4C996358, 0x63DA639C, 0x4B8E4C76, 0x33C03C18, 
0x6059601C, 0x48956098, 0x611A60D8, 0x4608615C, 0x487C6198, 
0x489261D8, 0x46106218, 0x62484619, 0x46184621, 0x48766281, 
0x62C84619, 0x63084875, 0x6348488C, 0x63884610, 0x63C84620, 
0x4870498A, 0x48706008, 0x48896048, 0x46106088, 0x462060C8, 
0x486B6108, 0x486B6148, 0x20BD6188, 0x61C804C0, 0x62084610, 
0x62484620, 0x62884865, 0x62C84865, 0x6308487F, 0x63484610, 
0x63884620, 0x63C84860, 0x48604979, 0x60083140, 0x497A4877, 
0x60413040, 0x46104975, 0x60883140, 0x60C84620, 0x61084858, 
0x61484858, 0x61884874, 0x61C84610, 0x62084620, 0x62484853, 
0x62884853, 0x62C84870, 0x63084610, 0x63484620, 0x6388484E, 
0x63C8484E, 0x486C4966, 0x60083180, 0x60484610, 0x60884620, 
0x60C84848, 0x61084848, 0x61484867, 0x61884610, 0x61C84620, 
0x62084843, 0x62484843, 0x62884863, 0x62C84610, 0x63084620, 
0x6348483E, 0x6388483E, 0x63C8485F, 0x46104955, 0x600831C0, 
0x60484620, 0x60884838, 0x60C84838, 0x6108485A, 0x61484610, 
0x61884620, 0x61C84833, 0x62084833, 0x62484856, 0x62884610, 
0x62C84620, 0x6308482E, 0x6348482E, 0x63884852, 0x63C84610, 
0x46204951, 0x48296008, 0x48296048, 0x484F6088, 0x461060C8, 
0x46206108, 0x48246148, 0x48246188, 0x200361C8, 0x0400494A, 
0x484A62C8, 0x494969C0, 0x61C843B0, 0x01C02061, 0x494861C8, 
0x60484846, 0x48474944, 0x49486148, 0x60084846, 0x49472055, 
0x61080200, 0x4846493F, 0x49446108, 0x61484845, 0x493E20FF, 
0x31403041, 0x49446048, 0x60084842, 0x2080493A, 0x60883140, 
0x48414601, 0xE0816101, 0x6A0391EF, 0x05ED89D9, 0x40022A80, 
0x05EEC4EC, 0x05F13B14, 0x05F27627, 0x05F3B13B, 0x05F4EC4F, 
0x05F62762, 0x05C6C4EC, 0x40022C00, 0x0000C890, 0x03600513, 
0x05C93B14, 0x05CA7627, 0x05CBB13B, 0x05CCEC4F, 0x05CE2762, 
0x05CF6276, 0x05D09D8A, 0x05D1D89E, 0x05D313B1, 0x05D44EC5, 
0x05D589D9, 0x40022D00, 0x05D6C4EC, 0x05D93B14, 0x05DA7627, 
0x05DBB13B, 0x05DCEC4F, 0x05DE2762, 0x05DF6276, 0x05E09D8A, 
0x05E1D89E, 0x05E313B1, 0x05E44EC5, 0x05E589D9, 0x40022E00, 
0x05E6C4EC, 0x05E93B14, 0x05EA7627, 0x05EBB13B, 0x05ECEC4F, 
0x05EE2762, 0x05EF6276, 0x05F09D8A, 0x05F1D89E, 0x05F313B1, 
0x05F44EC5, 0x05F589D9, 0x40022F00, 0x05F6C4EC, 0x40041A80, 
0x40020000, 0x00010001, 0x400250C0, 0x6A0091FF, 0x7F60010F, 
0x4000F400, 0x40020800, 0x0F141607, 0x0010108E, 0x08600140, 
0x40040080, 0x40025240, 0x49744A75, 0x21FF6111, 0x31A14A74, 
0x4A756391, 0x60114973, 0x21004A70, 0x60D13AC0, 0x4A716111, 
0x32804971, 0x49716011, 0x49716051, 0x49716091, 0x497160D1, 
0x03B26889, 0x4A6F4311, 0x4A666091, 0x3AC02101, 0x23FF6011, 
0x609333F9, 0x4A6B4966, 0x611139CD, 0x242960D1, 0x02E4495F, 
0x604C39C0, 0x4D674968, 0x4968600D, 0x608A2200, 0x36404E66, 
0x22CF6032, 0x02124F60, 0x4A64613A, 0x2709617A, 0x02BF4A5D, 
0x61173240, 0x4A614F62, 0x4F55603A, 0x603A4A53, 0x4A544F53, 
0x603A3780, 0x607A4A53, 0x60BA4A53, 0x60FA4A53, 0x4A5B4F53, 
0x4A5B607A, 0x4A5B603A, 0x4F5060BA, 0x60BA3740, 0x22014F46, 
0x603A3FC0, 0x4F4D4A48, 0x613A3ACD, 0x4F4260FA, 0x3F404A54, 
0x62FA63FA, 0x62BA627A, 0x321A4A51, 0x4F3E637A, 0x3F804A50, 
0x4A3A62BA, 0x3A1E4F3A, 0x22FF613A, 0x32814F38, 0x607A3740, 
0x60BA2208, 0x326D22FF, 0x22106002, 0x48336102, 0x608338C0, 
0x483C6044, 0x20006005, 0x60306088, 0x4B434844, 0x4B446143, 
0x4B446183, 0x4B4461C3, 0x493E618B, 0x620131F3, 0x62412100, 
0x2001492A, 0x61483180, 0x202D493F, 0x48336088, 0x6B013840, 
0x43990403, 0x63014319, 0x03D36B01, 0x43194399, 0x21216301, 
0x01494B2C, 0x6B016059, 0x031B2303, 0x63014399, 0x15436B01, 
0x63014399, 0x15836B01, 0x63014399, 0x23806B01, 0x43194399, 
0x6B016301, 0x43992360, 0x6B016301, 0x63014391, 0x08496B01, 
0x63010049, 0x6B414A12, 0x0C0932D4, 0x43110409, 0x6B416341, 
0x0412221F, 0x22014391, 0x43110452, 0x6B016341, 0x439103C2, 
0x63014311, 0xF874F000, 0x6800481D, 0xBDF84780, 0x00020020, 
0x400250C0, 0x40025280, 0xC30041A0, 0x40041800, 0x000100D0, 
0x00000A1E, 0x1E1E0101, 0x00001111, 0x40020800, 0x40024A00, 
0x000A1FFF, 0x40020180, 0x40024000, 0x0014148E, 0x08600140, 
0x40040080, 0x8228EE03, 0x00002252, 0x00000EEA, 0x00010208, 
0x0000022F, 0xCDDCF0FF, 0x40041900, 0x849DB9CD, 0x0000567E, 
0x00001D5A, 0x40020000, 0x100008F8, 0x4801B403, 0xBD019001, 
0x000140E9, 0x4801B403, 0xBD019001, 0x000003A1, 0x4801B403, 
0xBD019001, 0x000145ED, 0x4801B403, 0xBD019001, 0x00014487, 
0x4801B403, 0xBD019001, 0x00017D1D, 0x4801B403, 0xBD019001, 
0x000145F9, 0x4801B403, 0xBD019001, 0x000145FF, 0x4801B403, 
0xBD019001, 0x0001427F, 0x4801B403, 0xBD019001, 0x0000053F, 
0x4801B403, 0xBD019001, 0x00016629, 0x4801B403, 0xBD019001, 
0x000164BD, 0x4801B403, 0xBD019001, 0x00016E95, 0x00000000};



static void boot_delay(void)
{
	volatile uint32_t v1=0,v2=0,v3=0;

	for(v1=0;v1<50000;v1++)
	{
		for(v2=0;v2<10000;v2++)
		{
			v3 = v1 + v2;
			v3 = v3 - 2 + v1;
		}
	}
}

static void fw_patch_download(uint32_t* patch, uint32_t patch_size/*, void* plf_wr_apis*/)
{
	volatile uint32_t i=0, remaining=0;
	volatile uint32_t fp_val;

	remaining = patch_size;
	i=0;
	while(remaining > 128)
	{
		dbg_wr_mem_req_handler32(0x10008000+i*4, &patch[i], 128);
		i+=32;
		remaining -= 128;
	}
	dbg_wr_mem_req_handler32(0x10008000+i*4, &patch[i], (uint8_t)(remaining&0x000000FF));
		
	fp_val = 0x100081ad;
	dbg_wr_mem_req_handler32(0x10000004, (uint32_t *)&fp_val, 4);
	fp_val = 0x10008047;
	dbg_wr_mem_req_handler32(0x100000dc, (uint32_t *)&fp_val, 4);
	fp_val = 0x1000814d;
	dbg_wr_mem_req_handler32(0x100000e0, (uint32_t *)&fp_val, 4);
	fp_val = 0x100081d9;
	dbg_wr_mem_req_handler32(0x10000928, (uint32_t *)&fp_val, 4);
	fp_val = 0x10008d65;
	dbg_wr_mem_req_handler32(0x1000092c, (uint32_t *)&fp_val, 4);
	fp_val = 0x5fa0004;
	dbg_wr_mem_req_handler32_reset(0xe000ed0c, (uint32_t *)&fp_val, 4);
	
	#ifndef WIN32
		for(i=0; i<10000; i++)
			fp_val++;
	#else  
		boot_delay();
	#endif
}
#endif

at_ble_status_t at_ble_init(void* args)
{
	at_ble_status_t  status;

	status = platform_init(args);

	if(status == AT_BLE_SUCCESS)
	{
#ifndef __WINC3000__
		fw_patch_download((uint32_t*)fw_patch, sizeof(fw_patch));
#endif
		event_init();
		if(gapm_reset_req_handler() != ATT_ERR_NO_ERROR)
		{
			status = AT_BLE_FAILURE;
		}
	}

	// init device info
	if(status == AT_BLE_SUCCESS)
	{
		uint8_t loopCntr;
		device.srLen = 0;
		device.advLen = 0;
		device.role = ROLE_SLAVE;
		device.at_addr_set = false;
		device.privacy_flags = 0;
		device.addr_auto_gen = false;
		device.conn_handle = 0xFFFF;
		device.appearance = 0;
		device.spcp_param.con_intv_max = APP_CON_INTV_MIN;
		device.spcp_param.con_intv_min = APP_CON_INTV_MAX;
		device.spcp_param.con_latency = APP_CON_LATENCY;		
		device.spcp_param.superv_to = APP_SUPERV_TO;
		device.dev_name_write_perm = GAPM_WRITE_DISABLE;
		device.renew_dur = GAP_TMR_PRIV_ADDR_INT;
		at_ble_device_name_set((uint8_t *)GAP_device_name,sizeof(GAP_device_name));
		// TODO : used loopCntr value to avoid using rand() with samd21 applications
		for(loopCntr=0; loopCntr<AT_BLE_MAX_KEY_LEN; loopCntr++)
		{
			device.irk.key[loopCntr] = loopCntr;
		}
	}
	return status;
}
at_ble_status_t at_ble_set_privacy_key(at_ble_gap_irk_t* irk , uint16_t interval)
{
	if(interval == 0)
	{
		return AT_BLE_INVALID_PARAM;
	}
	else
	{
		device.renew_dur = interval;
	}
	if(irk != NULL)
	{
		memcpy(device.irk.key ,irk->irk , AT_BLE_MAX_KEY_LEN);
	}
	return AT_BLE_SUCCESS;
	

}

at_ble_status_t at_ble_set_gap_deviceinfo(at_ble_gap_deviceinfo_t*  gap_deviceinfo )
{
	if((gap_deviceinfo == NULL) || ((gap_deviceinfo->dev_name_perm) >AT_BLE_WRITE_AUTH))
	{
		return AT_BLE_INVALID_PARAM;
	}
	else
	{
		memcpy(&(device.spcp_param),&(gap_deviceinfo->spcp_param), sizeof(at_ble_spcp_t));
		device.appearance = gap_deviceinfo->appearance ;

		switch(gap_deviceinfo->dev_name_perm)
		{
		case AT_BLE_WRITE_DISABLE:
				device.dev_name_write_perm = GAPM_WRITE_DISABLE;
			break;
		case AT_BLE_WRITE_ENABLE:
			device.dev_name_write_perm = GAPM_WRITE_ENABLE;
			break;
		case AT_BLE_WRITE_UNAUTH:
			device.dev_name_write_perm = GAPM_WRITE_UNAUTH;
			break;
		case AT_BLE_WRITE_AUTH:
			device.dev_name_write_perm = GAPM_WRITE_AUTH;
			break;
		}
	}
	return AT_BLE_SUCCESS;
}
	
at_ble_status_t at_ble_device_name_set(uint8_t* dev_name, uint8_t len)
{
	at_ble_status_t status = AT_BLE_SUCCESS;
	do
	{
		if((dev_name == NULL) ||(len == 0))
		{
			status = AT_BLE_INVALID_PARAM;
			break;
		}
		
		if(gapm_set_dev_name_handler (len,dev_name) != ATT_ERR_NO_ERROR)
		{
			status = AT_BLE_FAILURE;
		}
	}while(0);

	return status;
}


at_ble_status_t at_ble_addr_set(at_ble_addr_t* address)
{
	uint8_t u8Status;
	at_ble_status_t status = AT_BLE_SUCCESS;

	do
	{
		if((address->addr == NULL) || 
			(address->type >AT_BLE_ADDRESS_RANDOM_PRIVATE_NON_RESOLVABLE))
		{
			status = AT_BLE_INVALID_PARAM;
			break;
		}
		if(address->type == AT_BLE_ADDRESS_PUBLIC)
		{
			u8Status = dbg_le_set_bd_addr_req_handler(address->addr);

			if(u8Status!=AT_BLE_SUCCESS)
			{
				status = AT_BLE_FAILURE;
				break;
			}
			memcpy(device.at_dev_addr.addr, address->addr ,AT_BLE_ADDR_LEN);
		}
		else if (address->type == AT_BLE_ADDRESS_RANDOM_STATIC)
		{
			if(address->addr != NULL)
			{
				memcpy(device.at_dev_addr.addr, address->addr ,AT_BLE_ADDR_LEN);
			}
			else
			{
				device.addr_auto_gen = true;
			}
		}
		else if ((address->type == AT_BLE_ADDRESS_RANDOM_PRIVATE_RESOLVABLE)
			||(address->type == AT_BLE_ADDRESS_RANDOM_PRIVATE_NON_RESOLVABLE))
		{
			device.privacy_flags = 1;
		}
		
		device.at_addr_set = true;
		device.at_dev_addr.type = address->type;
	}while(0);
	return status;
}

at_ble_status_t at_ble_addr_get(at_ble_addr_t* address)
{
  uint8_t addr[AT_BLE_ADDR_LEN+1];
  uint32_t chip_id = 0;
  uint32_t gap_dev_addr;

  if(address == NULL)
  {
	  return AT_BLE_INVALID_PARAM;
  }

  dbg_rd_mem_req_handler(0x4000B000,(uint8_t *)&chip_id,4);
  
  chip_id&=0xFFFFFF00;
  switch(chip_id)
  {
	  case 0x2000a000 :
	  gap_dev_addr = 0x10000E26;
	  break;
	  case 0x2000a200 :
	  gap_dev_addr = 0x10001096;
	  break;
	  case 0x2000a300 :
	  gap_dev_addr = 0x10001156;
	  break;
	  case 0x2000a400 :
	  gap_dev_addr = 0x1000085A;
	  break;
  }

  dbg_rd_mem_req_handler(gap_dev_addr, addr, AT_BLE_ADDR_LEN+1);
  address->type = AT_BLE_ADDRESS_PUBLIC;
  memcpy(address->addr, addr+1, 6);
  return AT_BLE_SUCCESS;
}

at_ble_status_t at_ble_adv_data_set( uint8_t const *const adv_data,
	uint8_t adv_data_len,uint8_t const *const scan_resp_data,
	uint8_t scan_response_data_len)
{
	at_ble_status_t status = AT_BLE_SUCCESS;
	
	do
	{
		if((adv_data== NULL) && (scan_resp_data == NULL))
		{
			status = AT_BLE_FAILURE;  
			break;
		}
		if((adv_data_len > AT_BLE_ADV_MAX_SIZE) || 
			(scan_response_data_len > AT_BLE_ADV_MAX_SIZE))
		{
			status = AT_BLE_INVALID_PARAM;
			break;
		}

	    if((adv_data != NULL) && (adv_data_len == 0) )
	    {
	       	// Clear advertising data
			memset(device.ADVData,0, AT_BLE_ADV_MAX_SIZE);
			device.advLen =  0;
	    }
		else if((adv_data != NULL) && (adv_data_len > 0))
		{
			// To do add advertising data check
			memcpy(device.ADVData,adv_data, adv_data_len);
			device.advLen =  adv_data_len;
		}

		if(scan_resp_data != NULL && scan_response_data_len == 0 )
	    {
	       	// Clear scan response data
			memset(device.SrData,0, AT_BLE_ADV_MAX_SIZE);
			device.srLen = 0;
	    }
		else if((scan_resp_data != NULL) && (scan_response_data_len > 0))
		{
			// To do add scan response data check
		    memcpy(device.SrData, scan_resp_data,scan_response_data_len);
			device.srLen = scan_response_data_len;
		}
	}while(0);
	return status;
}

at_ble_status_t at_ble_adv_start(at_ble_adv_type_t type,at_ble_adv_mode_t mode,
				at_ble_addr_t* peer_addr,at_ble_filter_type_t filtered, 
				uint16_t interval, uint16_t timeout, bool disable_randomness)
{
	uint8_t u8Status,adv_type,gatt_dev_addr , peer_addr_type = 0;
	at_ble_addr_t local_addr;
	uint8_t scan_rsp_len = device.srLen;

	at_ble_status_t status = AT_BLE_SUCCESS;

	// To do add timeout value for GAP
	do
	{
		if(((type != AT_BLE_ADV_TYPE_DIRECTED) && 
		   (device.at_dev_addr.type == AT_BLE_ADDRESS_RANDOM_PRIVATE_NON_RESOLVABLE))
		   ||(((type == AT_BLE_ADV_TYPE_DIRECTED) && 
		   (device.at_dev_addr.type == AT_BLE_ADDRESS_RANDOM_PRIVATE_RESOLVABLE))))
		{
			status = AT_BLE_PRIVACY_CFG_PB;
			break;
		}

		//If the advertising event type is either a discoverable undirected
		//event type or a non-connectable undirected event type, the advInterval shall
		//not be less than 100 ms. If the advertising event type is a connectable undirected
		//event type, the advInterval can be 20 ms or greater.
		if(((type == AT_BLE_ADV_TYPE_DIRECTED)&&(timeout != 0))
		||(type > AT_BLE_ADV_TYPE_NONCONN_UNDIRECTED)
		||(interval > AT_BLE_ADV_INTERVAL_MAX)
		||((type == AT_BLE_ADV_TYPE_UNDIRECTED)&&(interval < AT_BLE_ADV_INTERVAL_MIN)) 
		||((interval < AT_BLE_ADV_NONCON_INTERVAL_MIN) && 
		((type >= AT_BLE_ADV_TYPE_SCANNABLE_UNDIRECTED))) )
		{
			 status = AT_BLE_INVALID_PARAM;
			 break;
		};

		if((type == AT_BLE_ADV_TYPE_DIRECTED)&&
		(!(peer_addr->addr[0] || peer_addr->addr[1] || peer_addr->addr[2] 
		|| peer_addr->addr[3]|| peer_addr->addr[4] || peer_addr->addr[5])))
		{
			 status = AT_BLE_INVALID_PARAM;
			 break;
		}
		
		if((AT_BLE_ADV_TYPE_UNDIRECTED == type) && (AT_BLE_ADV_BROADCASTER_MODE == mode))
		{
			status = AT_BLE_INVALID_PARAM;
			break;
		}
		
		status = at_ble_addr_get(&local_addr);
		if(status!=AT_BLE_SUCCESS)
		{
			status = AT_BLE_FAILURE;
			break;
		}
		// To do check privacy setting and IRK key
		u8Status = gapm_set_dev_config_cmd_handler(GAP_PERIPHERAL_SLV,device.irk.key, 
				device.appearance, GAPM_WRITE_DISABLE, device.dev_name_write_perm, APP_MAX_MTU,
				device.spcp_param.con_intv_min, device.spcp_param.con_intv_max, device.spcp_param.con_latency, 
				device.spcp_param.superv_to, device.privacy_flags);
	
		if(u8Status!=AT_BLE_SUCCESS)
		{
			status = AT_BLE_FAILURE;
			break;
		}
		switch(type)
		{
			default:
			case  AT_BLE_ADV_TYPE_UNDIRECTED :
				adv_type = GAPM_ADV_UNDIRECT;
				break;
			case  AT_BLE_ADV_TYPE_DIRECTED:     
				adv_type = GAPM_ADV_DIRECT;
				break;
			/* differentiate between them by using scan response length*/
			case AT_BLE_ADV_TYPE_NONCONN_UNDIRECTED:
				scan_rsp_len = 0;
			case AT_BLE_ADV_TYPE_SCANNABLE_UNDIRECTED:     
				adv_type = GAPM_ADV_NON_CONN;
				break;
		}

		/*Own BD address source of the device */
		switch(local_addr.type)
		{
		case AT_BLE_ADDRESS_PUBLIC:
			gatt_dev_addr = GAPM_PUBLIC_ADDR;
			break;
		case AT_BLE_ADDRESS_RANDOM_STATIC:
			if(device.addr_auto_gen == true)
			{
				gatt_dev_addr = GAPM_PROVIDED_RND_ADDR;
			}
			else
			{
				gatt_dev_addr = GAPM_GEN_STATIC_RND_ADDR;
			}
			break;
		case AT_BLE_ADDRESS_RANDOM_PRIVATE_RESOLVABLE:
			gatt_dev_addr = GAPM_GEN_RSLV_ADDR;
			break;
		case AT_BLE_ADDRESS_RANDOM_PRIVATE_NON_RESOLVABLE:
			gatt_dev_addr = GAPM_PROVIDED_RECON_ADDR;
			break;
		}
		if(peer_addr != NULL)
		{
			if(peer_addr->type != AT_BLE_ADDRESS_PUBLIC)
			{
				peer_addr_type = 1;
			}
			else
			{
				peer_addr_type = 0;
			}
		}
		gapm_start_adv_cmd_handler(adv_type, gatt_dev_addr,
			device.renew_dur, local_addr.addr,peer_addr_type,(peer_addr==NULL ? NULL : peer_addr->addr),
			interval, interval, 7, mode, filtered,
			device.advLen, device.ADVData, scan_rsp_len, device.SrData);
		
	}while(0);
	return status;
}

at_ble_status_t at_ble_adv_stop(void)
{
	at_ble_status_t status = AT_BLE_SUCCESS;
	
	if(gapm_cancel_cmd_handler())
	{
		status = AT_BLE_FAILURE; 
	}
	return status;
}

at_ble_status_t at_ble_adv_set_tx_power(int8_t power)
{
	// To do check ble max connection number
	at_ble_status_t status = at_ble_tx_power_set(8,power);
	return status;
}
at_ble_status_t at_ble_scan_start(uint16_t interval,uint16_t window,uint16_t timeout,
	at_ble_scan_type_t type ,at_ble_scan_mode_t mode, bool filter_whitelist, 
	bool filter_dublicates)
{
	uint8_t scan_type,u8Status ,gatt_dev_addr;
	at_ble_addr_t local_addr;
	at_ble_status_t status = AT_BLE_SUCCESS;

	// To do support scan timeout
	do
	{
	    // if the scanning window is smaller or equal to the scanning interval
		if ((mode > AT_BLE_SCAN_OBSERVER_MODE)
		||(window > interval)
		||(type > AT_BLE_SCAN_ACTIVE)
		||(window > AT_BLE_SCAN_WINDOW_MAX)
		||(window < AT_BLE_SCAN_WINDOW_MIN)
		||(interval > AT_BLE_SCAN_INTERVAL_MAX)
		||(interval < AT_BLE_SCAN_INTERVAL_MIN))
		{
			 status = AT_BLE_INVALID_PARAM;
			 break;
		};
		
		status = at_ble_addr_get(&local_addr);
		if(status!=AT_BLE_SUCCESS)
		{
			status = AT_BLE_FAILURE;
			break;
		}
		u8Status = gapm_set_dev_config_cmd_handler(GAP_CENTRAL_MST, device.irk.key,device.appearance,
			GAPM_WRITE_DISABLE, device.dev_name_write_perm, APP_MAX_MTU, 0, 0, 0, 0, 0);

		if(u8Status!=AT_BLE_SUCCESS)
		{
			status = AT_BLE_FAILURE;
			break;
		}
		
		if(type == AT_BLE_SCAN_PASSIVE)
		{
			scan_type = GAPM_SCAN_PASSIVE; 
		}
		else
		{
			scan_type = GAPM_SCAN_ACTIVE;
		}

		/*Own BD address source of the device */
		switch(local_addr.type)
		{
		case AT_BLE_ADDRESS_PUBLIC:
			gatt_dev_addr = GAPM_PUBLIC_ADDR;
			break;
		case AT_BLE_ADDRESS_RANDOM_STATIC:
			if(device.addr_auto_gen == true)
			{
				gatt_dev_addr = GAPM_PROVIDED_RND_ADDR;
			}
			else
			{
				gatt_dev_addr = GAPM_GEN_STATIC_RND_ADDR;
			}
			break;
		case AT_BLE_ADDRESS_RANDOM_PRIVATE_RESOLVABLE:
			gatt_dev_addr = GAPM_GEN_RSLV_ADDR;
			break;
		case AT_BLE_ADDRESS_RANDOM_PRIVATE_NON_RESOLVABLE:
			gatt_dev_addr = GAPM_GEN_NON_RSLV_ADDR;
			break;
		}

		gapm_start_scan_cmd_handler(scan_type, gatt_dev_addr, 
			device.renew_dur,
			local_addr.addr, interval, window, mode, 
			filter_whitelist, filter_dublicates);

	}while(0);
	return status;
}

at_ble_status_t at_ble_scan_stop(void)
{
	at_ble_status_t status = AT_BLE_SUCCESS;
	
	if(gapm_cancel_cmd_handler())
	{
		status = AT_BLE_FAILURE; 
	}
	return status;
}

at_ble_status_t at_ble_connect(at_ble_addr_t peers[], uint8_t peer_count, 
	uint16_t scan_interval, uint16_t scan_window, at_ble_connection_params_t* connection_params)
{

	at_ble_status_t status = AT_BLE_SUCCESS;
	at_ble_addr_t local_addr;
	uint8_t gatt_dev_addr;
	do
	{
		/* check for the range validity of the values */
		if ( (connection_params->con_intv_max < AT_CNX_INTERVAL_MIN || 
		connection_params->con_intv_max > AT_CNX_INTERVAL_MAX) ||
		(connection_params->con_intv_min < AT_CNX_INTERVAL_MIN ||
		connection_params->con_intv_min > AT_CNX_INTERVAL_MAX) ||
		(connection_params->superv_to  < AT_CNX_SUP_TO_MIN   || 
		connection_params->superv_to  > AT_CNX_SUP_TO_MAX)   ||
		(connection_params->con_latency  > AT_CNX_LATENCY_MAX) )
		{
			status = AT_BLE_INVALID_PARAM;
			break;
		}

		device.role = ROLE_MASTER;

		status = at_ble_addr_get(&local_addr);
		if(status!=AT_BLE_SUCCESS)
		{
			status = AT_BLE_FAILURE;
			break;
		}
		status = gapm_set_dev_config_cmd_handler(GAP_CENTRAL_MST,device.irk.key,device.appearance,
		GAPM_WRITE_DISABLE, device.dev_name_write_perm, APP_MAX_MTU, 0, 0, 0, 0, 0);

		/*if(status!=AT_BLE_SUCCESS)
		{
			status = AT_BLE_FAILURE;
			break;
		}*/

		/*Own BD address source of the device */
		switch(local_addr.type)
		{
		case AT_BLE_ADDRESS_PUBLIC:
			gatt_dev_addr = GAPM_PUBLIC_ADDR;
			break;
		case AT_BLE_ADDRESS_RANDOM_STATIC:
			if(device.addr_auto_gen == true)
			{
				gatt_dev_addr = GAPM_PROVIDED_RND_ADDR;
			}
			else
			{
				gatt_dev_addr = GAPM_GEN_STATIC_RND_ADDR;
			}
			break;
		case AT_BLE_ADDRESS_RANDOM_PRIVATE_RESOLVABLE:
			gatt_dev_addr = GAPM_GEN_RSLV_ADDR;
			break;
		case AT_BLE_ADDRESS_RANDOM_PRIVATE_NON_RESOLVABLE:
			gatt_dev_addr = GAPM_GEN_NON_RSLV_ADDR;
			break;
		}

		gapm_start_connection_cmd_handler(GAPM_CONNECTION_AUTO,gatt_dev_addr, 
			device.renew_dur,local_addr.addr , scan_interval, scan_window,
			connection_params->con_intv_min, connection_params->con_intv_max,
			connection_params->con_latency,
			connection_params->superv_to,
			connection_params->ce_len_min,
			connection_params->ce_len_max,
			peer_count, peers);

	}while(0);
	return status;
}

at_ble_status_t at_ble_connect_cancel(void)
{
	at_ble_status_t status = AT_BLE_SUCCESS;
	
	if(gapm_cancel_cmd_handler())
	{
		status = AT_BLE_FAILURE; 
	}
	return status;
}

at_ble_status_t at_ble_sec_info_set(at_ble_handle_t handle, at_ble_signature_info_t signature_info, at_ble_auth_t auth, at_ble_author_t author)
{
	gapc_connection_cfm_handler(signature_info.local_csrk.key, signature_info.lsign_counter,
		signature_info.peer_csrk.key, signature_info.peer_sign_counter, auth,
					author, handle);
	return AT_BLE_SUCCESS;
}

at_ble_status_t at_ble_whitelist_add(at_ble_addr_t* address)
{
	at_ble_status_t status= AT_BLE_SUCCESS;
	if(address->addr == NULL)
	{
		return AT_BLE_FAILURE;
	}

	status = gapm_white_list_mgm_cmd(GAPM_ADD_DEV_IN_WLIST, 
		address->type, address->addr);

	return status;
}

at_ble_status_t at_ble_whitelist_remove(at_ble_addr_t* address)
{
	at_ble_status_t status= AT_BLE_SUCCESS;
	if(address->addr == NULL)
	{
		return AT_BLE_FAILURE;
	}

	status = gapm_white_list_mgm_cmd(GAPM_RMV_DEV_FRM_WLIST, 
		address->type, address->addr);

	return status;
}

at_ble_status_t at_ble_whitelist_clear(void)
{
	at_ble_status_t status= AT_BLE_SUCCESS;
	status= gapm_white_list_mgm_cmd(GAPM_CLEAR_WLIST, 
		0, NULL);

	return status;
}

at_ble_status_t at_ble_disconnect(at_ble_handle_t handle, at_ble_disconnect_reason_t reason)
{
	uint8_t gapc_reason ;
	switch(reason)
	{
		case AT_BLE_TERMINATED_BY_USER:
			gapc_reason = 0x13;
			break;
		case AT_BLE_UNACCEPTABLE_INTERVAL:
			gapc_reason = 0x3b;
			break;
		default:
			gapc_reason = 0x1F;
	}
	gapc_disconnect_cmd_handler(gapc_reason, handle);
	device.conn_handle = 0xFFFF;
	
	return AT_BLE_SUCCESS;
}


at_ble_status_t at_ble_connection_param_update(at_ble_handle_t handle, 
	at_ble_connection_params_t* connection_params)
{
	/* check for the range validity of the values */
	if ( (connection_params->con_intv_max < AT_CNX_INTERVAL_MIN || 
	connection_params->con_intv_max > AT_CNX_INTERVAL_MAX) ||
	(connection_params->con_intv_min < AT_CNX_INTERVAL_MIN ||
	connection_params->con_intv_min > AT_CNX_INTERVAL_MAX) ||
	(connection_params->superv_to  < AT_CNX_SUP_TO_MIN   || 
	connection_params->superv_to  > AT_CNX_SUP_TO_MAX)   ||
	(connection_params->con_latency  > AT_CNX_LATENCY_MAX) )
	{
		return AT_BLE_INVALID_PARAM;
	}

	gapc_param_update_cmd_handler(handle,
		connection_params->con_intv_min, connection_params->con_intv_max,
		connection_params->con_latency, connection_params->superv_to, 
		connection_params->ce_len_min, connection_params->ce_len_max);

	return AT_BLE_SUCCESS;
}

// To do check parameters
void at_ble_conn_update_reply(at_ble_handle_t conn_handle , 
				at_ble_connection_params_t* connection_params)
{
	gapc_param_update_cfm_handler(conn_handle, 
		connection_params->ce_len_min, connection_params->ce_len_max);
}

at_ble_status_t at_ble_tx_power_set(at_ble_handle_t conn_handle, float power)
{
	uint32_t level;
	if(power<=(-16.5))
		level=1;
	else if(power<=(-11))
		level=2;
	else if(power<=(-7.9))
		level=3;
	else if(power<=(-5.9))
		level=4;
	else if(power<=(-4.3))
		level=5;
	else if(power<=(-3.2))
		level=6;
	else if(power<=(-2.4))
		level=7;
	else if(power<=(-1.7))
		level=8;
	else if(power<=(-1.2))
		level=9;
	else if(power<=(-0.7))
		level=10;
	else if(power<=(0.1))
		level=12;
	else if(power<=(0.6))
		level=14;
	else if(power<=(1))
		level=16;
	else if(power<=(1.4))
		level=18;
	else if(power<=(1.6))
		level=20;
	else if(power<=(1.8))
		level=22;
	else if(power<=(2))
		level=24;
	else if(power<=(2.1))
		level=26;
	else if(power<=(2.3))
		level=28;
	else if(power<=(2.4))
		level=30;
	else if(power<=(2.6))
		level=34;
	else if(power<=(2.7))
		level=38;
	else if(power<=(2.8))
		level=42;
	else if(power<=(3))
		level=50;
	else
		level=63;
	/// Power table
//	static const int8_t RF_RPL_TX_PW_CONV_TBL[RPL_PWR_TBL_SIZE] 
//			= {-23,-20,-17,-14,-11,-8,-5,-2};


/*	for(level = RPL_POWER_MIN; level <= RPL_POWER_MAX; level++)
	{
		if(RF_RPL_TX_PW_CONV_TBL[level] >= power)
			break;
	}

	if(level > RPL_POWER_MAX)
	{
		level = RPL_POWER_MAX;
	}
*/
	if(dbg_wr_mem_req_handler32(0x4002084c, (uint32_t*)&level, 1) != AT_BLE_SUCCESS)
	{
		return AT_BLE_FAILURE;
	}
	else
	{
		return AT_BLE_SUCCESS;
	}
}

float at_ble_tx_power_get(at_ble_handle_t conn_handle)
{
	uint32_t level=0;
	double power;
	dbg_rd_mem_req_handler32(0x4002084c, (uint8_t*)&level, 4);
	level=level>>8;
	level&=0x3f;
	if(level<=1)
		power=(-16.5);
	else if(level==2)
		power=(-11);
	else if(level==3)
		power=(-7.9);
	else if(level==4)
		power=(-5.9);
	else if(level==5)
		power=(-4.3);
	else if(level==6)
		power=(-3.2);
	else if(level==7)
		power=(-2.4);
	else if(level==8)
		power=(-1.7);
	else if(level==9)
		power=(-1.2);
	else if(level==10)
		power=(-0.7);
	else if(level<=12)
		power=(0.1);
	else if(level<=14)
		power=(0.6);
	else if(level<=16)
		power=(1);
	else if(level<=18)
		power=(1.4);
	else if(level<=20)
		power=(1.6);
	else if(level<=22)
		power=(1.8);
	else if(level<=24)
		power=(2);
	else if(level<=26)
		power=(2.1);
	else if(level<=28)
		power=(2.3);
	else if(level<=30)
		power=(2.4);
	else if(level<=34)
		power=(2.6);
	else if(level<=38)
		power=(2.7);
	else if(level<=42)
		power=(2.8);
	else if(level<=50)
		power=(3);
	else
		power=3.1;

	return (float)power;
}

at_ble_status_t at_ble_random_address_resolve(uint8_t nb_key, at_ble_addr_t* rand_addr, uint8_t* irk_key)
{
	at_ble_status_t status = AT_BLE_SUCCESS;

	do
	{
		if((rand_addr == NULL) || (irk_key == NULL) ||(nb_key == 0)||
			(rand_addr->type != AT_BLE_ADDRESS_RANDOM_PRIVATE_RESOLVABLE ))
		{
			status = AT_BLE_INVALID_PARAM;
			break;
		}

		gapm_resolv_addr_cmd_handler(nb_key , rand_addr->addr,irk_key);
	}while(0);
	return status;
}


int8_t at_ble_rx_power_get(at_ble_handle_t conn_handle)
{
	at_ble_events_t event;
	uint8_t params[1];
	uint16_t rssi;

	gapc_get_info_cmd_handler(conn_handle, GAPC_GET_CON_RSSI);

	do
	{
		if (at_ble_event_get(&event, params, -1) == AT_BLE_SUCCESS)
		{
			if (event == AT_BLE_RX_POWER_VALUE)
			{
				gapc_con_rssi_ind* p = (gapc_con_rssi_ind* )params;
				rssi = p->rssi;
			}
		}
	}while(event != AT_BLE_RX_POWER_VALUE);

	if(rssi > 127)
	{
		rssi -= 255;
	}

	return (int8_t)rssi;
}
