
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
	0x2101B510, 0x07094802, 0xF0006048, 0xBD10FDA9, 0x00018405,
	0xF000B538, 0x4C1BFDA9, 0xD01F2800, 0x68A84D1A, 0x60284780,
	0x61204819, 0x47806868, 0xF0002003, 0x2000FDA1, 0x48169000,
	0x00D2227D, 0x06C96881, 0x9900D404, 0x91001C49, 0xD3F74291,
	0x06C06880, 0x4910D404, 0x064A6808, 0x60084310, 0xFD90F000,
	0x6881480D, 0x43912238, 0x68016081, 0x43112201, 0x480A6001,
	0x20036220, 0x46106160, 0xFD86F000, 0x0000BD38, 0x4000E000,
	0x1000020C, 0x000001FF, 0x4000E300, 0x4000F040, 0x40025000,
	0x002000C0, 0xB5F04770, 0xB08B49FF, 0x60CA220C, 0x60CB2308,
	0x60CB2300, 0x23BC4CFC, 0x4DFD60A3, 0x606B4BFB, 0x60634BFC,
	0x60636863, 0x4BFB4EFC, 0x698B60B3, 0x618B4313, 0x21004AFA,
	0x085B6813, 0x6013005B, 0x62424AF8, 0x62824AF8, 0x23064AF8,
	0x22306142, 0x4AF75413, 0x4AF762C2, 0x4AF76002, 0x4AF76202,
	0x4AF76042, 0x48F761C2, 0x237F6802, 0x439A061B, 0x069B2303,
	0x6002431A, 0x23FF6802, 0x439A041B, 0x045B2321, 0x6002431A,
	0x230F6802, 0x439A021B, 0x021B2307, 0x6002431A, 0x22566803,
	0x021B0A1B, 0x60034313, 0x0048466A, 0x1C495450, 0x2928B2C9,
	0x48E5D3F9, 0x210269C0, 0x49E34308, 0x68E861C8, 0x43082102,
	0x60C849D2, 0x48E04BE1, 0x4AE16018, 0x48E1605A, 0x49E16098,
	0x49E160D9, 0x4EE16119, 0x619A615E, 0x4EDD61D8, 0x6259621E,
	0x629E4EDE, 0x631862DA, 0x1E764ED9, 0x6399635E, 0x63DE4EDB,
	0x33404BD3, 0x6058601A, 0x4ED14BD4, 0x36401E5B, 0x60F160B3,
	0x61334BD6, 0x61B06172, 0x1E5B4BCF, 0x623161F3, 0x62734BD3,
	0x62F062B2, 0x1E9B4BCB, 0x63716333, 0x63B34BD0, 0x4BC563F2,
	0x60183380, 0x4EC34BC6, 0x36801E9B, 0x60B16073, 0x60F34BCB,
	0x61706132, 0x1E9B4BC1, 0x61F161B3, 0x051B235D, 0x62726233,
	0x4BBD62B0, 0x62F31EDB, 0x4BC46331, 0x63B26373, 0x4BB963F0,
	0x1EDB4EB5, 0x603336C0, 0x4BC06071, 0x60F260B3, 0x4BB46130,
	0x61731EDB, 0x4BBD61B1, 0x623261F3, 0x4BB06270, 0x62B31F1B,
	0x4BBA62F1, 0x63726333, 0x4BAC63B0, 0x63F31F1B, 0x60194BB7,
	0x4BB74EB6, 0x60B26073, 0x4BA760F0, 0x61331F1B, 0x4BB46171,
	0x61F261B3, 0x4BA36230, 0x62731F1B, 0x4BB162B1, 0x633262F3,
	0x63584633, 0x1F5B4B9E, 0x63F163B3, 0x4BAD4EA9, 0x60333640,
	0x60B06072, 0x1F5B4B99, 0x613160F3, 0x61734BA9, 0x61F061B2,
	0x1F5B4B95, 0x62716233, 0x62B34BA6, 0x463362F2, 0x4B916318,
	0x63731F9B, 0x4BA363B1, 0x4B9B63F3, 0x601A3380, 0x4B8C6058,
	0x1F9B4E98, 0x60B33680, 0x4B9E60F1, 0x61726133, 0x4B8761B0,
	0x61F31F9B, 0x232F6231, 0x6273055B, 0x62F062B2, 0x1FDB4B82,
	0x63716333, 0x4E964F8D, 0x63BE3780, 0x4E8B63FA, 0x603036C0,
	0x60B16073, 0x4E924F88, 0x60FE37C0, 0x6178613A, 0x61F961BB,
	0x623E4E8F, 0x62B8627A, 0x633962FB, 0x637B4B8D, 0x63F863BA,
	0x4B8C4E72, 0x601E3E08, 0x4F8B6059, 0x60DA609F, 0x615E6118,
	0x4F896199, 0x621A61DF, 0x629E6258, 0x4E8762D9, 0x635A631E,
	0x4E686398, 0x63DE3E09, 0x33404B80, 0x4F836019, 0x609A605F,
	0x611E60D8, 0x4F816159, 0x61DA619F, 0x625E6218, 0x4E7F6299,
	0x631A62DE, 0x4E5D6358, 0x639E3E0A, 0x4B7563D9, 0x33804F7B,
	0x605A601F, 0x60DE6098, 0x4A796119, 0x4A54615A, 0x61D8619A,
	0x6259621E, 0x0500205F, 0x62DA6298, 0x46194850, 0x46306308,
	0x48506348, 0x48716388, 0x486763C8, 0x600230C0, 0x6041494A,
	0x60811E71, 0x60C1494A, 0x6101496C, 0x49466142, 0x1E716181,
	0x494661C1, 0x49696201, 0x62826241, 0x62C14941, 0x63011E71,
	0x63414941, 0x63814965, 0x496563C2, 0x6008483C, 0x60481EB0,
	0x6088483C, 0x49624861, 0x610260C1, 0x4837495F, 0x1EB06148,
	0x48376188, 0x4B5F61C8, 0x6018485D, 0x4832605A, 0x6098380D,
	0x6119495C, 0x04F626B9, 0x619A615E, 0x625961D8, 0x629E4E59,
	0x631862DA, 0x4E586399, 0x4B5463DE, 0x601A3340, 0x60D96058,
	0x611E4E55, 0x6198615A, 0x4E546219, 0x629A625E, 0x635962D8,
	0x639E4E52, 0x4B4B63DA, 0x60183380, 0x4E506099, 0x611A60DE,
	0x61D96158, 0x621E4E4E, 0x6298625A, 0x4E4D6319, 0x639A635E,
	0x4B4263D8, 0x605933C0, 0xE0954B4A, 0x4000E200, 0x4000F400,
	0x00020042, 0x4000F000, 0x31888C02, 0x66656867, 0x4000E040,
	0x40041800, 0x0001769B, 0x0001768D, 0x00017403, 0x0001740B,
	0x000173D5, 0x000173EF, 0x000173F1, 0x100080A5, 0x40040080,
	0x40020000, 0x05C62762, 0x40022800, 0x0F141607, 0x6A029154,
	0x00009D82, 0x0F60010B, 0x05C76276, 0x05C89D8A, 0x05C9D89E,
	0x05CB13B1, 0x05CC4EC5, 0x05CD89D9, 0x05CEC4EC, 0x05D13B14,
	0x05D27627, 0x05D3B13B, 0x05D4EC4F, 0x40022900, 0x05D62762,
	0x05D76276, 0x05D89D8A, 0x05D9D89E, 0x05DB13B1, 0x05DC4EC5,
	0x05DD89D9, 0x05DEC4EC, 0x05E13B14, 0x05E27627, 0x05E3B13B,
	0x05E4EC4F, 0x40022A00, 0x05E62762, 0x05E76276, 0x05E89D8A,
	0x05E9D89E, 0x05EB13B1, 0x05EC4EC5, 0x05ED89D9, 0x05EEC4EC,
	0x05F13B14, 0x05F27627, 0x05F3B13B, 0x05F4EC4F, 0x40022B00,
	0x05F62762, 0x05C6C4EC, 0x40022C00, 0x03600311, 0x05C93B14,
	0x05CA7627, 0x05CBB13B, 0x05CCEC4F, 0x05CE2762, 0x05CF6276,
	0x05D09D8A, 0x05D1D89E, 0x05D313B1, 0x36C04EF8, 0x60F260B3,
	0x61B16130, 0x61F34BF6, 0x62706232, 0x4BF562F1, 0x63726333,
	0x4BF463B0, 0x4EF46019, 0x609A605E, 0x615960D8, 0x04F626BB,
	0x61DA619E, 0x62996218, 0x62DE4EEF, 0x6358631A, 0x4BEB63D9,
	0x33404EED, 0x605A601E, 0x61196098, 0x615E4EEB, 0x61D8619A,
	0x4EEA6259, 0x62DA629E, 0x63996318, 0x63DE4EE8, 0x33804BE1,
	0x6058601A, 0x4EE660D9, 0x615A611E, 0x62196198, 0x625E4EE4,
	0x62D8629A, 0x4EE36359, 0x63DA639E, 0x33C04BD8, 0x60996018,
	0x60DE4EE0, 0x6158611A, 0x4EDF61D9, 0x625A621E, 0x63196298,
	0x635E4EDD, 0x63D8639A, 0x60594BDC, 0x4BDC4EDB, 0x60F260B3,
	0x61B16130, 0x04DB23BD, 0x623261F3, 0x62F16270, 0x63334BD7,
	0x63B06372, 0x33404BD3, 0x4ED56019, 0x609A605E, 0x615960D8,
	0x619E4ED3, 0x621861DA, 0x4ED26299, 0x631A62DE, 0x63D96358,
	0x4ED04BCA, 0x601E3380, 0x6098605A, 0x4ECE6119, 0x619A615E,
	0x625961D8, 0x629E4ECC, 0x631862DA, 0x4ECB6399, 0x4BC163DE,
	0x601A33C0, 0x60D96058, 0x611E4EC8, 0x6198615A, 0x4EC76219,
	0x629A625E, 0x635962D8, 0x639E4EC5, 0x4BC563DA, 0x60996018,
	0x4BC44EC3, 0x613260F3, 0x61F16170, 0x623149C2, 0x62B06272,
	0x633048C1, 0x49C120FF, 0x600830C9, 0x49C048BF, 0x49C06041,
	0x49BD6081, 0x60C848BD, 0x610F2700, 0x618F614F, 0x200361CF,
	0x040049BB, 0x48B662C8, 0x20616020, 0x01C049B6, 0x61C83920,
	0x48B749B8, 0x48B86208, 0x48B86248, 0x48B86288, 0x48B862C8,
	0x48B86308, 0x48B86348, 0x48B86388, 0x49B063C8, 0x314048B7,
	0x48B76008, 0x49B86048, 0x604848B6, 0x49B72011, 0x610802C0,
	0x614848B6, 0x49B42009, 0x31400280, 0x48B56108, 0x600149B3,
	0x49B448B5, 0x48AD6001, 0x30CF49B3, 0x60083180, 0x608848B2,
	0x60C848B2, 0x48B249AA, 0x49A96088, 0x60883140, 0x200149A6,
	0x600839C0, 0x49AE48A3, 0x61081C80, 0x49A260C8, 0x394048AC,
	0x62C863C8, 0x62886248, 0x301A48A9, 0x49AA6348, 0x628848A8,
	0x48A9499B, 0x20FF6108, 0x30814999, 0x60483140, 0x60882001,
	0x200249A3, 0x63083140, 0x306D20FF, 0x20106008, 0x20FF6108,
	0x30F94991, 0x608839C0, 0x02C02029, 0x489D6048, 0x489C6087,
	0x60073040, 0x489B499C, 0x489C6148, 0x489C6188, 0x499761C8,
	0x6188489B, 0x49972097, 0x62080080, 0x498A624F, 0x31802001,
	0x49736148, 0x3920202D, 0x4E846088, 0x6B303E40, 0x43880149,
	0x63304308, 0x10C96B30, 0x43084388, 0x20216330, 0x0140497D,
	0x6B306048, 0x03092103, 0x63304388, 0x15716B30, 0x63304388,
	0x10496B30, 0x63304388, 0x21806B30, 0x43084388, 0x6B306330,
	0x43882160, 0x6B306330, 0x43882110, 0x6B306330, 0x00400840,
	0x6B706330, 0x0C00497D, 0x43080400, 0x6B706370, 0x0409211F,
	0x21014388, 0x43080449, 0x6B306370, 0x43880109, 0x63304308,
	0xF94EF000, 0xF952F000, 0xF956F000, 0x03F16B30, 0x63304388,
	0x10C96B30, 0x43084388, 0x6B306330, 0x43880109, 0x486C6330,
	0x47806800, 0x486B60AF, 0x486B6007, 0x21046980, 0x49694308,
	0x69886188, 0x43082108, 0x61884966, 0x68004864, 0x43082180,
	0x60084962, 0x68014608, 0xD4FC0609, 0x4A606901, 0x03FF2701,
	0x43B96993, 0x43B32608, 0x69936193, 0x43B32604, 0x68026193,
	0x431A2380, 0x68026002, 0xD4FC0612, 0x43BA6902, 0x68626102,
	0x330123FF, 0x6062439A, 0x690624E1, 0x43BE0264, 0xF0004851,
	0x4607F911, 0x46214377, 0xF0004638, 0xB282F90B, 0x43504620,
	0x21001A3B, 0xE0980920, 0x40022C00, 0x05D44EC5, 0x05D589D9,
	0x40022D00, 0x05D6C4EC, 0x05D93B14, 0x05DA7627, 0x05DBB13B,
	0x05DCEC4F, 0x05DE2762, 0x05DF6276, 0x05E09D8A, 0x05E1D89E,
	0x05E313B1, 0x05E44EC5, 0x05E589D9, 0x40022E00, 0x05E6C4EC,
	0x05E93B14, 0x05EA7627, 0x05EBB13B, 0x05ECEC4F, 0x05EE2762,
	0x05EF6276, 0x05F09D8A, 0x05F1D89E, 0x05F313B1, 0x05F44EC5,
	0x05F589D9, 0x40022F00, 0x05F6C4EC, 0x05C6C4EC, 0x7F600007,
	0x40023000, 0x40020020, 0x100001C8, 0x40041A80, 0x03020100,
	0x40030000, 0x07060504, 0x0B0A0908, 0x0F0E0D0C, 0x13121110,
	0x17161514, 0x1B1A1918, 0x1F1E1D1C, 0x23222120, 0x27262524,
	0x00010001, 0x400250C0, 0x40020800, 0x001414CE, 0x08600140,
	0x40040080, 0xC30041A0, 0x40041800, 0x1E1E0101, 0x00001111,
	0x00000EE5, 0x40024A00, 0x00010208, 0x0000022F, 0x40025200,
	0x00020002, 0x40024000, 0xCCDCF0FF, 0x40041900, 0x7289A1B8,
	0x00005A59, 0x00001D5A, 0x00000AF2, 0x10000218, 0x4000C000,
	0x4000B000, 0x018CBA80, 0xB2C91C49, 0x42981A1B, 0x1C49D3FA,
	0x4812084B, 0x62012100, 0x00D16281, 0x62414319, 0x490F4810,
	0x6A0161C1, 0x0A4922FF, 0x32C90249, 0x02C24311, 0x62014391,
	0x62014311, 0x6068480A, 0x200F490A, 0x480960C8, 0x38404909,
	0x49096041, 0x490A6001, 0x60484808, 0xBDF0B00B, 0x40004000,
	0x03007080, 0x40020000, 0x00020043, 0x40020840, 0x74488E43,
	0x000022D2, 0x00000303, 0x40041880, 0x4801B403, 0xBD019001,
	0x0000061D, 0x4801B403, 0xBD019001, 0x00014685, 0x4801B403,
	0xBD019001, 0x00014631, 0x4801B403, 0xBD019001, 0x00014673,
	0x4801B403, 0xBD019001, 0x0001464F, 0x4801B403, 0xBD019001,
	0x000168BD, 0x4801B403, 0xBD019001, 0x00016751, 0x4801B403,
	0xBD019001, 0x00017129, 0x4801B403, 0xBD019001, 0x00018009};


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
	while(remaining > (128/1))
	{
		dbg_wr_mem_req_handler32(0x10008000+i*4, &patch[i], (128/1));
		i+=(32/1);
		remaining -= (128/1);
	}

	dbg_wr_mem_req_handler32(0x10008000+i*4, &patch[i], (uint8_t)(remaining&0x000000FF));
		
	fp_val = 0x10008001;
	dbg_wr_mem_req_handler32(0x10000004, (uint32_t *)&fp_val, 4);
	fp_val = 0;
	dbg_wr_mem_req_handler32(0x100000e4, (uint32_t *)&fp_val, 4);
	fp_val = 0x10008015;
	dbg_wr_mem_req_handler32(0x10000218, (uint32_t *)&fp_val, 4);
	fp_val = 0x100080a7;
	dbg_wr_mem_req_handler32(0x1000024c, (uint32_t *)&fp_val, 4);
	fp_val = 0x20101985;
	dbg_wr_mem_req_handler32(0x10000000, (uint32_t *)&fp_val, 4);
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
	memcpy(address->addr, addr+1, 6);
	address->type = AT_BLE_ADDRESS_PUBLIC;

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
