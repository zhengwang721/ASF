
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
	0x2101B510, 0x07094802, 0xF0006048, 0xBD10FDCB, 0x00018119,
	0xF000B510, 0x2000FDCB, 0xFDCEF000, 0xFDD2F000, 0xFDD6F000,
	0xFDDAF000, 0x20004901, 0xBD106108, 0x40040040, 0x4604B510,
	0x481B2106, 0xFDD4F000, 0xFDD8F000, 0x01522229, 0x20004918,
	0xFDD8F000, 0x02922203, 0x20014916, 0xFDD2F000, 0x015222D7,
	0x20024914, 0xFDCCF000, 0x02922205, 0x20034912, 0xFDC6F000,
	0xF0002002, 0xF000FDC9, 0x490FFDCD, 0x6809480A, 0x47883838,
	0xFFC2F7FF, 0xFDCAF000, 0x49052001, 0x70C82C00, 0x4620D002,
	0xFDC8F000, 0xFDCCF000, 0x0000BD10, 0x10000918, 0x10002D38,
	0x10003264, 0x10003E70, 0x1000595C, 0x10000928, 0x4FFFB5F0,
	0x6879B08B, 0x431115BA, 0x4AFD6079, 0x68132100, 0x005B085B,
	0x4AFB6013, 0x4AFB6242, 0x4AFB6282, 0x61422306, 0x54132230,
	0x62C24AF9, 0x60024AF9, 0x62024AF9, 0x60424AF9, 0x4BFA4AF9,
	0x681861C2, 0x0612227F, 0x22034390, 0x43100692, 0x68186018,
	0x041222FF, 0x22214390, 0x43100452, 0x68186018, 0x0212220F,
	0x22074390, 0x43100212, 0x681A6018, 0x0A122056, 0x43020212,
	0x4668601A, 0x5442004A, 0xB2C91C49, 0xD3F92928, 0x69C048E7,
	0x43082102, 0x61C849E5, 0x200D49E5, 0x48E560C8, 0x22036841,
	0x60414311, 0x220268C1, 0x60C14311, 0x48E14DE2, 0x4CE26028,
	0x48E2606C, 0x49E260A8, 0x4AE260E9, 0x4EE2612A, 0x61AC616E,
	0x622961E8, 0x4EE0626A, 0x62EC62AE, 0x63696328, 0x4EDE63AA,
	0x4DD663EE, 0x602C3540, 0x60A96068, 0x4ED360EA, 0x36404DDA,
	0x61746135, 0x61F161B0, 0x4DD86232, 0x62B46275, 0x633162F0,
	0x4DD66372, 0x63F463B5, 0x35804DCA, 0x60696028, 0x4EC860AA,
	0x36804DD2, 0x613460F5, 0x61B16170, 0x255D61F2, 0x6235052D,
	0x62B06274, 0x633262F1, 0x63754DCC, 0x63F063B4, 0x35C04DBE,
	0x606A6029, 0x4DC94EBC, 0x60B536C0, 0x613060F4, 0x61B26171,
	0x61F54DC6, 0x62706234, 0x62F262B1, 0x63354DC4, 0x63B06374,
	0x4DC363F1, 0x4EC2602A, 0x60754DC2, 0x60F060B4, 0x61726131,
	0x61B54DC0, 0x623061F4, 0x62B26271, 0x62F54DBE, 0x63706334,
	0x463563B1, 0x4DB863EA, 0x35404EBB, 0x606C602E, 0x60E960A8,
	0x4EB9612A, 0x61AC616E, 0x622961E8, 0x4EB7626A, 0x62EC62AE,
	0x63696328, 0x4EB563AA, 0x4DAD63EE, 0x602C3580, 0x60A96068,
	0x4EB260EA, 0x616C612E, 0x61E961A8, 0x262F622A, 0x626E0576,
	0x62E862AC, 0x636A6329, 0x63AE4EAC, 0x4DA263EC, 0x602835C0,
	0x60AA6069, 0x60EE4EA9, 0x6168612C, 0x61EA61A9, 0x622E4EA7,
	0x62A8626C, 0x632A62E9, 0x636E4EA5, 0x63E863AC, 0x60294DA4,
	0x4EA4606A, 0x60EC60AE, 0x61696128, 0x4EA261AA, 0x622C61EE,
	0x62A96268, 0x4EA062EA, 0x636C632E, 0x63E963A8, 0x35404D9A,
	0x4E9D602A, 0x60AC606E, 0x612960E8, 0x4E9B616A, 0x61EC61AE,
	0x62696228, 0x4E9962AA, 0x632C62EE, 0x63A96368, 0x4D9063EA,
	0x35804E96, 0x606C602E, 0x60E960A8, 0x4994612A, 0x46216169,
	0x61E861A9, 0x6228486F, 0x62684610, 0x0500205F, 0x460862A8,
	0x62C84629, 0x63084869, 0x63484869, 0x63884610, 0x4628498A,
	0x498063C1, 0x31C04620, 0x48636008, 0x48636048, 0x487C6088,
	0x30C04611, 0x497A60C1, 0x31C04883, 0x46206108, 0x485C6148,
	0x485C6188, 0x461061C8, 0x487F6208, 0x48736248, 0x30C04621,
	0x49716281, 0x31C04855, 0x485562C8, 0x46106308, 0x48796348,
	0x46206388, 0x497863C8, 0x6008484F, 0x6048484F, 0x60884610,
	0x60C84875, 0x61084620, 0x6148484A, 0x6188484A, 0x61C84610,
	0x48714972, 0x1EE56008, 0x4871604D, 0x4A6F6088, 0x60D14970,
	0x4A704C6D, 0x24B96122, 0x04E44E6B, 0x61B56174, 0x623161F0,
	0x4C6C6272, 0x462C62B4, 0x633062F4, 0x63B26371, 0x63F44C69,
	0x462E4C63, 0x60263440, 0x60A16060, 0x4E6660E2, 0x462E6126,
	0x61A06166, 0x622261E1, 0x62664E63, 0x62A6462E, 0x632162E0,
	0x4E616362, 0x462E63A6, 0x4C5763E6, 0x60203480, 0x60A26061,
	0x60E64E5D, 0x6126462E, 0x61A16160, 0x4E5B61E2, 0x462E6226,
	0x62A06266, 0x632262E1, 0x63664E58, 0x63A6462E, 0x4C4B63E0,
	0x602134C0, 0x4E556062, 0x462E60A6, 0x612060E6, 0x61A26161,
	0x61E64E52, 0x6226462E, 0x62A16260, 0x4E5062E2, 0x462E6326,
	0x63A06366, 0x4C4E63E1, 0x4E4E6022, 0x462E6066, 0xE09960A6,
	0x4000F400, 0x40041800, 0x00017407, 0x000173F9, 0x0001716F,
	0x00017177, 0x00017141, 0x0001715B, 0x0001715D, 0x0001712B,
	0x40040080, 0x40020000, 0x40020840, 0x4000F000, 0x05C62762,
	0x40022800, 0x0F141A07, 0x6A0391EB, 0x0000D3A0, 0x00600113,
	0x05C76276, 0x05C89D8A, 0x05C9D89E, 0x05CB13B1, 0x05CC4EC5,
	0x05CD89D9, 0x05CEC4EC, 0x05D13B14, 0x05D27627, 0x05D3B13B,
	0x05D4EC4F, 0x40022900, 0x05D62762, 0x05D76276, 0x05D89D8A,
	0x05D9D89E, 0x05DB13B1, 0x05DC4EC5, 0x05DD89D9, 0x05DEC4EC,
	0x05E13B14, 0x05E27627, 0x05E3B13B, 0x05E4EC4F, 0x40022A00,
	0x05E62762, 0x05E76276, 0x05E89D8A, 0x05E9D89E, 0x05EB13B1,
	0x05EC4EC5, 0x05ED89D9, 0x05EEC4EC, 0x05F13B14, 0x05F27627,
	0x05F3B13B, 0x05F4EC4F, 0x40022B00, 0x05F62762, 0x05C6C4EC,
	0x40022C00, 0x6A0091D7, 0x0000C890, 0x0360060F, 0x05C93B14,
	0x05CA7627, 0x05CBB13B, 0x05CCEC4F, 0x05CE2762, 0x05CF6276,
	0x05D09D8A, 0x05D1D89E, 0x05D313B1, 0x05D44EC5, 0x05D589D9,
	0x40022D00, 0x05D6C4EC, 0x612160E0, 0x26BB6162, 0x61A604F6,
	0x61E6462E, 0x62616220, 0x4EFE62A2, 0x462E62E6, 0x63606326,
	0x63E263A1, 0x4EFB4CFC, 0x462E6026, 0x60A06066, 0x612260E1,
	0x61664EF9, 0x61A6462E, 0x622161E0, 0x4EF76262, 0x462E62A6,
	0x632062E6, 0x63A26361, 0x63E64EF4, 0x462E4CF0, 0x60263440,
	0x60A16060, 0x4EF160E2, 0x462E6126, 0x61A06166, 0x622261E1,
	0x62664EEE, 0x62A6462E, 0x632162E0, 0x4EEC6362, 0x462E63A6,
	0x4CE463E6, 0x60203480, 0x60A26061, 0x60E64EE8, 0x6126462E,
	0x61A16160, 0x4EE661E2, 0x462E6226, 0x62A06266, 0x632262E1,
	0x63664EE3, 0x63A6462E, 0x4CD863E0, 0x602134C0, 0x4EE06062,
	0x462E60A6, 0x612060E6, 0x61A26161, 0x04D222BD, 0x622561E2,
	0x62A16260, 0x62E048DA, 0x462148DA, 0x634E6308, 0x638848D9,
	0x63C848D9, 0x48D549D9, 0x48D96008, 0x46286048, 0x48D46088,
	0x48D560C8, 0x610149D3, 0x614149CF, 0x48D449D2, 0x46286188,
	0x48D061C8, 0x620149CD, 0x624149CD, 0x48C949CD, 0x48CF6288,
	0x462862C8, 0x48C86308, 0x48C86348, 0x48C46388, 0x49C763C8,
	0x314048CA, 0x46286008, 0x48C26048, 0x48C36088, 0x304049C1,
	0x49BD60C1, 0x49C56101, 0x49BF6141, 0x31404628, 0x48BD6188,
	0x304049BA, 0x49BB61C1, 0x314048B9, 0x48B56208, 0x48BE6248,
	0x48B76288, 0x30404629, 0x49B562C1, 0x314048B2, 0x48B26308,
	0x48B26348, 0x304049AD, 0x49B06381, 0x314048B6, 0x49AE63C8,
	0x31804628, 0x48AA6008, 0x48AA6048, 0x48A66088, 0x48B160C8,
	0x46286108, 0x48A56148, 0x48A56188, 0x48A161C8, 0x48AD6208,
	0x46286248, 0x48A06288, 0x48A062C8, 0x489C6308, 0x48A96348,
	0x46286388, 0x499D63C8, 0x31C0489A, 0x489A6008, 0x48966048,
	0x48A46088, 0x462860C8, 0x48956108, 0x48956148, 0x48916188,
	0x200361C8, 0x0400499F, 0x489F62C8, 0x210269C0, 0x499D4388,
	0x206161C8, 0x61C801C0, 0x489B499C, 0x489C6208, 0x489C6248,
	0x489C6288, 0x489C62C8, 0x489C6308, 0x489C6348, 0x489C6388,
	0x499463C8, 0x3140489B, 0x489B6008, 0x499C6048, 0x6048489A,
	0x498D487E, 0x61483028, 0x60384899, 0x49992055, 0x61080200,
	0x48984988, 0x49966108, 0x61484897, 0x499220FF, 0x31403041,
	0x48956048, 0x20806018, 0x49946088, 0x498D6108, 0x61084893,
	0x499120FF, 0x314030A1, 0x497B6388, 0x62C84890, 0x49904891,
	0x49866001, 0x39C02000, 0x610860C8, 0x498D4882, 0x318030CF,
	0x488C6008, 0x488C6048, 0x488C6088, 0x488060C8, 0x21016880,
	0x430803C9, 0x6088497D, 0x2001497A, 0x600839C0, 0x30F920FF,
	0x49766088, 0x1C894A84, 0x60D16111, 0x49742329, 0x39C002DB,
	0x4D82604B, 0x602C4C80, 0x22004981, 0x4E80608A, 0x60323640,
	0x4F6F22CF, 0x613A0212, 0x617A4A7D, 0x4F6C2209, 0x37400292,
	0x4F7B613A, 0x603A4A6C, 0x4A6F4F70, 0x4A64603A, 0x32CF4F6E,
	0x603A3780, 0x607A4A6D, 0x60BA4A6D, 0x60FA4A6D, 0x4A734F61,
	0x4F6060BA, 0x60BA3740, 0x22014F5C, 0x603A3FC0, 0x4F684A59,
	0x613A1C92, 0x4F5860FA, 0x3F404A6C, 0x62FA63FA, 0x62BA627A,
	0x321A4A69, 0x4F59637A, 0x3F404A68, 0x4A5862BA, 0x3A1E4F50,
	0x22FF613A, 0x32814F4E, 0x607A3740, 0x60BA2208, 0x4F5122FF,
	0x603A326D, 0x613A2210, 0x3FC04F48, 0x607B60B8, 0x2000602C,
	0x60306088, 0x4B5B485C, 0x4B5C6143, 0x4B5C6183, 0x4B5C61C3,
	0x2197618B, 0x62010089, 0x62412100, 0x20014947, 0x61483180,
	0x212D482E, 0x4C4D6081, 0x6B203C40, 0x43A80495, 0x63204328,
	0x03D16B20, 0x43084388, 0x20216320, 0x01404946, 0x6B206048,
	0x03092103, 0x63204388, 0x01516B20, 0x63204388, 0x10496B20,
	0x63204388, 0xE08D6B20, 0x05D93B14, 0x05DA7627, 0x40022D40,
	0x05DBB13B, 0x05DCEC4F, 0x05DE2762, 0x05DF6276, 0x05E09D8A,
	0x05E1D89E, 0x05E313B1, 0x05E44EC5, 0x05E589D9, 0x05E6C4EC,
	0x0360060F, 0x05E93B14, 0x6A0091D7, 0x0000C890, 0x40022E40,
	0x05EA7627, 0x05EBB13B, 0x05ECEC4F, 0x05EE2762, 0x05EF6276,
	0x05F09D8A, 0x05F1D89E, 0x05F313B1, 0x05F44EC5, 0x05F589D9,
	0x05F6C4EC, 0x40041A80, 0x40020000, 0x03020100, 0x40030000,
	0x07060504, 0x0B0A0908, 0x0F0E0D0C, 0x13121110, 0x17161514,
	0x1B1A1918, 0x1F1E1D1C, 0x23222120, 0x27262524, 0x00010001,
	0x400250C0, 0x7F60010F, 0x40020800, 0x0F141607, 0x0010108E,
	0x08600140, 0x40025240, 0x00020020, 0x0000E090, 0xC30041A0,
	0x40041800, 0x00000A1E, 0x1E1E0101, 0x00001111, 0x40024A00,
	0x000A1FFF, 0x40020180, 0x40024000, 0x0014148E, 0x40040080,
	0x00000EEA, 0x00010208, 0x0000022F, 0xCCDCF0FF, 0x40041900,
	0x7289A1B8, 0x00005A59, 0x00001D5A, 0x43882180, 0x63204308,
	0x21606B20, 0x63204388, 0x43906B20, 0x6B206320, 0x00400840,
	0x48156320, 0x0C096B61, 0x43010409, 0x6B606361, 0x0409211F,
	0x03514388, 0x63604308, 0x010E6B20, 0x433043B0, 0xF0006320,
	0xF000F871, 0xF000F875, 0x6B20F879, 0x632043B0, 0x11296B20,
	0x43084388, 0x6B206320, 0x632043A8, 0x68004804, 0x49044780,
	0x60882000, 0xBDF0B00B, 0x00000AF2, 0x100008F8, 0x4000F000,
	0x4801B403, 0xBD019001, 0x0000053F, 0x4801B403, 0xBD019001,
	0x00014E35, 0x4801B403, 0xBD019001, 0x000028CD, 0x4801B403,
	0xBD019001, 0x0000099D, 0x4801B403, 0xBD019001, 0x000047BB,
	0x4801B403, 0xBD019001, 0x00014FB5, 0x4801B403, 0xBD019001,
	0x00017DC7, 0x4801B403, 0xBD019001, 0x0001593D, 0x4801B403,
	0xBD019001, 0x00015AA9, 0x4801B403, 0xBD019001, 0x00000359,
	0x4801B403, 0xBD019001, 0x00015531, 0x4801B403, 0xBD019001,
	0x0000E731, 0x4801B403, 0xBD019001, 0x0000E77D, 0x4801B403,
	0xBD019001, 0x0001468D, 0x4801B403, 0xBD019001, 0x00016629,
	0x4801B403, 0xBD019001, 0x000164BD, 0x4801B403, 0xBD019001,
0x00016E95};


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
		
	fp_val = 0x10008001;
	dbg_wr_mem_req_handler32(0x10000004, (uint32_t *)&fp_val, 4);
	fp_val = 0x100080c5;
	dbg_wr_mem_req_handler32(0x10000928, (uint32_t *)&fp_val, 4);
	fp_val = 0x10008039;
	dbg_wr_mem_req_handler32(0x10000920, (uint32_t *)&fp_val, 4);
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
	#define AT_BT_DEFAULT_BDADDR "\x01\x23\x45\x67\x89\xAB"

	if(address == NULL)
	{
		return AT_BLE_INVALID_PARAM;
	}

	if(device.at_addr_set == true)
	{
		address->type = device.at_dev_addr.type;
		memcpy(address->addr, device.at_dev_addr.addr,AT_BLE_ADDR_LEN);
	}
	else
	{
		// To do check device default address
		address->type = AT_BLE_ADDRESS_PUBLIC;
		memcpy(address->addr,(uint8_t *)AT_BT_DEFAULT_BDADDR,AT_BLE_ADDR_LEN);
	}
	
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
