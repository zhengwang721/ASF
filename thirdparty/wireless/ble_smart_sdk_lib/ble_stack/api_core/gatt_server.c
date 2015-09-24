#include "at_ble_api.h"
#include "gattm_task.h"
#include "gattc_task.h"
#include "string.h"
//#include "error.h"

#include "cmn_defs.h"

volatile static uint8_t primary_service_uuid[] = {0x00, 0x28};
volatile static uint8_t secondary_service_uuid[] = {0x01, 0x28};
static uint8_t include_uuid[] = {0x02, 0x28};
static uint8_t char_uuid[] = {0x03, 0x28};

static uint8_t extended_props_uuid[] = {0x00, 0x29};
static uint8_t user_desc_uuid[] = {0x01, 0x29};
static uint8_t client_conf_uuid[] = {0x02, 0x29};
static uint8_t server_conf_uuid[] = {0x03, 0x29};
static uint8_t presentation_format_uuid[] = {0x04, 0x29};

uint8_t unNull = 0;

#include "interface.h"
//#define RI_FLAG           ((uint16_t)1<<15)

int Log_FN=0;

#define TEMP_FN_IN() Log_FN=__LINE__

typedef enum
{
    NOT_AUTHZ = 0x00,
    READ_AUTHZ = 0x01,
    WRITE_AUTHZ = 0x02,
} Authz_t;

typedef struct
{
    uint16_t handle;
    Authz_t Authz_type;
    uint16_t length;
    uint8_t *data;
} valueElem_t;

//uint8_t * pu8trdbMemory=0;
//uint8_t * pu8tr_dbMemoryEnd=0;

uint32_t u32dbMemorySize = 0;
uint32_t u32numberOfValues = 0;
uint8_t *pu8valuesBuffer = 0;
uint8_t   *pu8valuesBufferEnd = 0;
valueElem_t *ptstrattDb = 0;

void db_init(uint8_t *memPool, uint32_t memSize)
{
    UnionPtr uptr;
    TEMP_FN_IN();
    u32dbMemorySize = memSize;
    pu8valuesBufferEnd = pu8valuesBuffer = memPool;
    memPool += memSize;
    ALIGN_PTR_WORD_FLOOR_BOUNDARY(memPool);
    // it is perefared to create memPool 32 bit aligned
    uptr.u8ptr = (uint8_t *)memPool;
    ptstrattDb = (valueElem_t *)uptr.u32ptr;
    FN_OUT(0);
}

static at_ble_status_t db_addATT(uint16_t handle, uint8_t *pu8val, int length, at_ble_attr_permissions_t prop)
{
    TEMP_FN_IN();
    /*Allocating Memory*/
    if (ptstrattDb)
    {
        ptstrattDb = &ptstrattDb[-1]; /*This array (pointer) grow descendingly */
        if (pu8val)
        {
            pu8valuesBufferEnd = &pu8valuesBufferEnd[length];
        }
        /*Adding the element*/
        if ((void *)ptstrattDb > (void *)pu8valuesBufferEnd)
        {
            ptstrattDb[0].handle = handle;
            ptstrattDb[0].data = &pu8valuesBufferEnd[-length];
            if (pu8val && length)
            {
                ptstrattDb[0].data = &pu8valuesBufferEnd[-length];
                ptstrattDb[0].length = length;
                memcpy(ptstrattDb[0].data, pu8val, length);
            }
            else
            {
                ptstrattDb[0].data = NULL;
                ptstrattDb[0].length = length;
            }
            ptstrattDb[0].Authz_type = NOT_AUTHZ;
            if ((prop & AT_BLE_ATTR_READABLE_NO_AUTHN_REQ_AUTHR) == AT_BLE_ATTR_READABLE_NO_AUTHN_REQ_AUTHR
                    ||
                    (prop & AT_BLE_ATTR_READABLE_REQ_AUTHN_REQ_AUTHR) == AT_BLE_ATTR_READABLE_REQ_AUTHN_REQ_AUTHR)
            {
                ptstrattDb[0].Authz_type |= READ_AUTHZ;
            }
            if ((prop & AT_BLE_ATTR_WRITABLE_NO_AUTHN_REQ_AUTHR) == AT_BLE_ATTR_WRITABLE_NO_AUTHN_REQ_AUTHR
                    ||
                    (prop & AT_BLE_ATTR_WRITABLE_REQ_AUTHN_REQ_AUTHR) == AT_BLE_ATTR_WRITABLE_REQ_AUTHN_REQ_AUTHR)
            {
                ptstrattDb[0].Authz_type |= WRITE_AUTHZ;
            }
            u32numberOfValues++;
            FN_OUT(AT_BLE_SUCCESS);
            return AT_BLE_SUCCESS;
        }
        else
        {
            /*Restoring the pointers*/
            ptstrattDb = &ptstrattDb[1]; /*This array (pointer) grow descendingly */
            if (pu8val)
            {
                pu8valuesBufferEnd = &pu8valuesBufferEnd[-length];
            }
            FN_OUT(AT_BLE_ATT_INSUFF_RESOURCE);
            return AT_BLE_ATT_INSUFF_RESOURCE;
        }
    }
    else
    {
        FN_OUT(AT_BLE_DB_MEMORY_NOT_SETTED);
        return AT_BLE_DB_MEMORY_NOT_SETTED;
    }
}

static valueElem_t *db_searchATT(uint16_t handle)
{
    int rangeStart = 0;
    int rangeEnd = u32numberOfValues;
    TEMP_FN_IN();
    while (rangeStart <= rangeEnd)
    {
        int midRange = (rangeStart + rangeEnd) / 2;
        if (ptstrattDb[midRange].handle < handle)
        {
            rangeEnd = midRange - 1;
        }
        else if (ptstrattDb[midRange].handle > handle)
        {
            rangeStart = midRange + 1;
        }
        else
        {
            FN_OUT(0);
            return &ptstrattDb[midRange];
        }
    }
    FN_OUT(0);
    return NULL;
}


static bool is_extended_properties_defined(
    at_ble_char_properties_t  properties)
{
    /*
    an extended properties descriptor is needed if either
    "reliable write" or "writable aux" are defined
    */
    TEMP_FN_IN();
    FN_OUT(0);
    return (properties
            & (AT_BLE_CHAR_RELIABLE_WRITE | AT_BLE_CHAR_WRITEABLE_AUX)) != 0;
}

static uint16_t att_permissions(at_ble_attr_permissions_t permissions, char Read, char Write)
{
    uint16_t perm = 0;
    TEMP_FN_IN();
    if ((permissions & AT_BLE_ATTR_READABLE_NO_AUTHN_REQ_AUTHR)
            == AT_BLE_ATTR_READABLE_NO_AUTHN_REQ_AUTHR)
    {
        perm = PERM(RD, AUTHZ);
    }
    else if ((permissions & AT_BLE_ATTR_READABLE_REQ_AUTHN_REQ_AUTHR)
             == AT_BLE_ATTR_READABLE_REQ_AUTHN_REQ_AUTHR)
    {
        perm = PERM(RD, AUTHZ) | PERM(RD, AUTH);
    }
    else if ((permissions & AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR)
             == AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR)
    {
        perm = PERM(RD, AUTH);
    }
    else
    {
        if (Read)
        {
            perm = PERM(RD, ENABLE);
        }
    }
    if ((permissions & AT_BLE_ATTR_WRITABLE_NO_AUTHN_REQ_AUTHR)
            == AT_BLE_ATTR_WRITABLE_NO_AUTHN_REQ_AUTHR)
    {
        perm |= PERM(WR, AUTHZ);
        perm |= (1 << 13);
    }
    else if ((permissions & AT_BLE_ATTR_WRITABLE_REQ_AUTHN_REQ_AUTHR)
             == AT_BLE_ATTR_WRITABLE_REQ_AUTHN_REQ_AUTHR)
    {
        perm |= PERM(WR, AUTHZ) | PERM(WR, AUTH);
        perm |= (1 << 13);
    }
    else if ((permissions & AT_BLE_ATTR_WRITABLE_REQ_AUTHN_NO_AUTHR)
             == AT_BLE_ATTR_WRITABLE_REQ_AUTHN_NO_AUTHR)
    {
        perm |= PERM(WR, AUTH);
        perm |= (1 << 13);
    }
    else
    {
        if (Write)
        {
            perm |= PERM(WR, ENABLE);
        }
        perm |= (1 << 13);
    }
    FN_OUT(0);
    return perm;
}
#define RI_FLAG(Permissions) ((((Permissions&AT_BLE_ATTR_READABLE_REQ_AUTHN_REQ_AUTHR)==AT_BLE_ATTR_READABLE_REQ_AUTHN_REQ_AUTHR)|((Permissions&AT_BLE_ATTR_READABLE_NO_AUTHN_REQ_AUTHR)==AT_BLE_ATTR_READABLE_NO_AUTHN_REQ_AUTHR))<<15)

static uint8_t user_description_define
(at_ble_handle_t att_handle, at_ble_attr_permissions_t perm,
 uint8_t *user_data, uint16_t len)
{
    TEMP_FN_IN();
    if (user_data != NULL)
    {
        if (RI_FLAG(perm))
        {
            db_addATT(att_handle, user_data, len, perm);
        }
        else
        {
            db_addATT(att_handle, NULL, 0, perm);
            gattm_att_set_value_req_handler(att_handle, len, user_data);
        }
    }
    else
    {
        ERROR_REPORT;
    }
    FN_OUT(0);
    return AT_BLE_SUCCESS;
}

static bool is_server_conf_defined(
    at_ble_char_properties_t properties)
{
    TEMP_FN_IN();
    FN_OUT(0);
    return (properties & AT_BLE_CHAR_BROADCST) != 0;
}

static uint8_t server_conf_define(at_ble_handle_t att_handle, at_ble_attr_permissions_t perm)
{
    uint8_t temp_buff[2] = {0x00, 0x00};
    TEMP_FN_IN();
    gattm_att_set_value_req_handler(att_handle, 2, temp_buff);
    db_addATT(att_handle, temp_buff, 2, perm);
    FN_OUT(0);
    return AT_BLE_SUCCESS;
}

static bool is_client_conf_defined(
    at_ble_char_properties_t  properties)
{
    TEMP_FN_IN();
    FN_OUT(0);
    return (properties
            & (AT_BLE_CHAR_NOTIFY | AT_BLE_CHAR_INDICATE)) != 0;
}

static uint8_t client_conf_define(at_ble_handle_t att_handle, at_ble_attr_permissions_t perm)
{
    uint8_t temp_buff[2] = {0x00, 0x00};
    TEMP_FN_IN();
    gattm_att_set_value_req_handler(att_handle, 2, temp_buff);
    db_addATT(att_handle, temp_buff, 2, perm);
    FN_OUT(0);
    return AT_BLE_SUCCESS;
}



static uint8_t presentation_format_define(at_ble_handle_t att_handle,
        at_ble_char_presentation_t *ppresentation_format)
{
    uint8_t temp_buff[7];
    TEMP_FN_IN();
    temp_buff[0] = ppresentation_format->format;
    temp_buff[1] = ppresentation_format->exponent;
    temp_buff[2] = ppresentation_format->unit & 0xff;
    temp_buff[3] = ppresentation_format->unit >> 8;
    temp_buff[4] = ppresentation_format->name_space;
    temp_buff[5] = ppresentation_format->description & 0xff;
    temp_buff[6] = ppresentation_format->description >> 8;
    gattm_att_set_value_req_handler(att_handle, 7, temp_buff);
    FN_OUT(0);
    return AT_BLE_SUCCESS;
}





static uint16_t char_permissions(at_ble_char_properties_t props, at_ble_attr_permissions_t perms)
{
    uint16_t perm = 0;
    TEMP_FN_IN();
    // TODO: proper permission mapping based on characteristic->value_permissions
    if ((props & AT_BLE_CHAR_NOTIFY) == AT_BLE_CHAR_NOTIFY)
    {
        perm |= PERM(NTF, ENABLE);
    }
    if ((props & AT_BLE_CHAR_INDICATE) == AT_BLE_CHAR_INDICATE)
    {
        perm |= PERM(IND, ENABLE);
    }
    if ((props & AT_BLE_CHAR_SIGNED_WRITE) == AT_BLE_CHAR_SIGNED_WRITE)
    {
        perm |= PERM(WR, ENABLE) | PERM(WRITE_SIGNED, ENABLE);
        perm |= (1 << 13);
    }
    if ((props & AT_BLE_CHAR_WRITEABLE_AUX) == AT_BLE_CHAR_WRITEABLE_AUX)
    {
        perm |= PERM(EXT, ENABLE);
        perm |= (1 << 13);
    }
    if ((props & AT_BLE_CHAR_RELIABLE_WRITE) == AT_BLE_CHAR_RELIABLE_WRITE)
    {
        perm |= PERM(BROADCAST, ENABLE);
        perm |= (1 << 13);
    }
    if ((perms & AT_BLE_ATTR_READABLE_NO_AUTHN_REQ_AUTHR)
            == AT_BLE_ATTR_READABLE_NO_AUTHN_REQ_AUTHR)
    {
        perm |= PERM(RD, ENABLE);
    }
    else if ((perms & AT_BLE_ATTR_READABLE_REQ_AUTHN_REQ_AUTHR)
             == AT_BLE_ATTR_READABLE_REQ_AUTHN_REQ_AUTHR)
    {
        perm |=  PERM(RD, AUTH);
    }
    else if ((perms & AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR)
             == AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR)
    {
        perm |= PERM(RD, AUTH);
    }
    else
    {
        if ((props & AT_BLE_CHAR_READ) == AT_BLE_CHAR_READ)
        {
            perm |= PERM(RD, ENABLE);
        }
    }
    if ((perms & AT_BLE_ATTR_WRITABLE_NO_AUTHN_REQ_AUTHR)
            == AT_BLE_ATTR_WRITABLE_NO_AUTHN_REQ_AUTHR)
    {
        perm |= PERM(WR, ENABLE);
        perm |= (1 << 13);
    }
    else if ((perms & AT_BLE_ATTR_WRITABLE_REQ_AUTHN_REQ_AUTHR)
             == AT_BLE_ATTR_WRITABLE_REQ_AUTHN_REQ_AUTHR)
    {
        perm |=  PERM(WR, AUTH);
        perm |= (1 << 13);
    }
    else if ((perms & AT_BLE_ATTR_WRITABLE_REQ_AUTHN_NO_AUTHR)
             == AT_BLE_ATTR_WRITABLE_REQ_AUTHN_NO_AUTHR)
    {
        perm |= PERM(WR, AUTH);
        perm |= (1 << 13);
    }
    else
    {
        if ((props & AT_BLE_CHAR_RELIABLE_WRITE) ==  AT_BLE_CHAR_RELIABLE_WRITE)
        {
            perm |= PERM(WR, ENABLE);
            perm |= (1 << 13);
        }
        if ((props & AT_BLE_CHAR_WRITE_WITHOUT_RESPONSE) == AT_BLE_CHAR_WRITE_WITHOUT_RESPONSE)
        {
            perm |= PERM(WR, ENABLE);
            perm |= (1 << 11);
        }
        if ((props & AT_BLE_CHAR_WRITE) == AT_BLE_CHAR_WRITE)
        {
            perm |= PERM(WR, ENABLE);
            perm |= (1 << 13);
        }
    }
    FN_OUT(0);
    return perm;
}


static void att_permPackUUID(uint16_t *ptr_perm, at_ble_uuid_type_t uuidType)
{
    TEMP_FN_IN();
    switch (uuidType)
    {
    case AT_BLE_UUID_16:
        *ptr_perm |= PERM(UUID_LEN, 16_BIT);
        break;
    case AT_BLE_UUID_32:
        *ptr_perm |= PERM(UUID_LEN, 32_BIT);
        break;
    case AT_BLE_UUID_128:
        *ptr_perm |= PERM(UUID_LEN, 128_BIT);
        break;
    default:
        break;
    }
    FN_OUT(0);
}

static void cpy_UUID_IntoBuffer(uint8_t *buffer, at_ble_uuid_t *uuid)
{
    TEMP_FN_IN();
    memcpy(buffer, uuid->uuid, at_ble_uuid_type2len(uuid->type));
    FN_OUT(0);
}



at_ble_status_t at_ble_service_define(at_ble_service_t *svc)
{
    UnionPtr uptr;
    TEMP_FN_IN();
    if (svc)
    {
        int i = 0;
        uint16_t  start_hdl = 0, Accumulated_handle = 0;
        uint16_t temp_handle;
        uint16_t temp_perm;
        uint8_t status;
        struct gattm_svc_desc *svc_desc;
        // use the same buffer to collect with safety offset 10 bytes
        uptr.u8ptr = (uint8_t *)&interface_send_msg[20/*must be multibles of 2(Largest alignment requirement of any member) to be sure from Alignment*/];
        svc_desc = (struct gattm_svc_desc *)uptr.u32ptr;
        svc_desc->start_hdl = 0;
        svc_desc->task_id = TASK_GTL;
        memset(svc_desc->uuid, 0, ATT_UUID_128_LEN);
        cpy_UUID_IntoBuffer(svc_desc->uuid, &svc->uuid);
        svc_desc->perm = 0x04 | (svc->uuid.type << 4) | (svc->type << 6);
        svc_desc->nb_att = 0;
        do
        {
            /* then go through include services */
            for (i = 0; i < svc->included_count; i++)
            {
                /*uuid_size(&included_list[i].uuid) + 4*/
                status = gattm_add_attribute_req_handler(svc_desc, svc->inc_list[i].service_handle, PERM(RD, ENABLE),
                         sizeof(include_uuid), include_uuid, &temp_handle);
            }
            /* Finally the characteristic list */
            if (svc->char_list)
                for (i = 0; i < svc->char_count; i++)
                {
                    uint32_t j = 0;
                    at_ble_chr_t *charactaristic = &svc->char_list[i];
                    /* define "char decl" attribute */
                    status = gattm_add_attribute_req_handler(svc_desc, 4 /*No meaning:
                    For Characteristic Declarations this field is not used*/,
                             PERM(RD, ENABLE),
                             sizeof(char_uuid), char_uuid, &temp_handle);
                    if (status != AT_BLE_SUCCESS)
                    {
                        break;
                    }
                    temp_perm = char_permissions(charactaristic->char_val.properties, charactaristic->char_val.permissions);
                    att_permPackUUID(&temp_perm, charactaristic->char_val.uuid.type);
                    /* define the attribute holding the value */
                    status = gattm_add_attribute_req_handler(svc_desc, RI_FLAG(charactaristic->char_val.permissions) | charactaristic->char_val.len, temp_perm ,
                             at_ble_uuid_type2len(charactaristic->char_val.uuid.type), charactaristic->char_val.uuid.uuid, &charactaristic->char_val.handle);
                    if (status != AT_BLE_SUCCESS)
                    {
                        break;
                    }
                    /* Finally the descriptors */
                    if (is_extended_properties_defined(svc->char_list[i].char_val.properties))
                    {
                        uint8_t temp_buff[2] = {0x00, 0x00};
                        if ((charactaristic->char_val.properties & AT_BLE_CHAR_RELIABLE_WRITE) == AT_BLE_CHAR_RELIABLE_WRITE)
                        {
                            temp_buff[0] |= 0x01;
                        }
                        if ((charactaristic->char_val.properties & AT_BLE_CHAR_WRITEABLE_AUX) == AT_BLE_CHAR_WRITEABLE_AUX)
                        {
                            temp_buff[0] |= 0x02;
                        }
                        gattm_add_attribute_req_handler(svc_desc, ((uint16_t)temp_buff[0]) << 8 | temp_buff[1], PERM(RD, ENABLE),
                                                        sizeof(extended_props_uuid), extended_props_uuid, &temp_handle);
                    }
                    if (svc->char_list[i].user_desc.len)
                    {
                        uint16_t perm = PERM(RD, ENABLE);
                        if ((charactaristic->char_val.properties & AT_BLE_CHAR_WRITEABLE_AUX) == AT_BLE_CHAR_WRITEABLE_AUX)
                        {
                            perm = att_permissions(charactaristic->user_desc.permissions, 1, 1);
                        }
                        else
                        {
                            perm = att_permissions(charactaristic->user_desc.permissions, 1, 0);
                        }
                        gattm_add_attribute_req_handler(svc_desc, charactaristic->user_desc.len, RI_FLAG(charactaristic->user_desc.permissions) | perm,
                                                        sizeof(user_desc_uuid), user_desc_uuid, &charactaristic->user_desc.handle);
                    }
                    if (is_server_conf_defined(svc->char_list[i].char_val.properties))
                    {
                        gattm_add_attribute_req_handler(svc_desc, 2, att_permissions(charactaristic->server_config_desc.perm, 1, 0),
                                                        sizeof(server_conf_uuid), server_conf_uuid, &charactaristic->server_config_desc.handle);
                    }
                    if (is_client_conf_defined(svc->char_list[i].char_val.properties))
                    {
                        gattm_add_attribute_req_handler(svc_desc, 2, att_permissions(charactaristic->client_config_desc.perm, 1, 1),
                                                        sizeof(client_conf_uuid), client_conf_uuid, &charactaristic->client_config_desc.handle);
                    }
                    if (svc->char_list[i].presentation_format)
                    {
                        gattm_add_attribute_req_handler(svc_desc, 7, PERM(RD, ENABLE),
                                                        sizeof(presentation_format_uuid), presentation_format_uuid, &temp_handle);
                    }
                    for (j = 0; j < charactaristic->additional_desc_count; j++)
                    {
                        gattm_add_attribute_req_handler(svc_desc, charactaristic->additional_desc_list[j].desc_val_length, charactaristic->additional_desc_list[j].perm,
                                                        sizeof(charactaristic->additional_desc_list[j].uuid), charactaristic->additional_desc_list[j].uuid.uuid, &charactaristic->additional_desc_list[j].handle);
                    }
                }
            status = gattm_add_svc_req_handler(svc_desc);
            if (status != AT_BLE_SUCCESS)
            {
                break;
            }
            svc->handle = svc_desc->start_hdl;
            start_hdl = svc_desc->start_hdl;
            Accumulated_handle = start_hdl;
            /* define "include" attributes */
            for (i = 0; i < svc->included_count; i++)
            {
                Accumulated_handle++;
            }
            /* define characteristics */
            if (svc->char_list)
                for (i = 0; i < svc->char_count; i++)
                {
                    at_ble_chr_t *charactaristic = &svc->char_list[i];
                    Accumulated_handle++;
                    Accumulated_handle++;
                    charactaristic->char_val.handle = Accumulated_handle;
                    if (charactaristic->char_val.len > 0)
                    {
#define P_DB(X,Y) (RI_FLAG(X)? Y: NULL)/*Present in DB*/
                        //status = at_ble_characteristic_value_set(charactaristic->char_val_handle,
                        //  charactaristic->init_value,0,charactaristic->value_init_len);
                        if (RI_FLAG(charactaristic->char_val.permissions))
                        {
                            db_addATT(charactaristic->char_val.handle,
                                      UN_NULL(charactaristic->char_val.init_value),
                                      charactaristic->char_val.len,
                                      charactaristic->char_val.permissions);
                        }
                        else
                        {
                            db_addATT(charactaristic->char_val.handle,
                                      NULL, 0, charactaristic->char_val.permissions);
                            gattm_att_set_value_req_handler(charactaristic->char_val.handle, charactaristic->char_val.len,
                                                            charactaristic->char_val.init_value);
                            if (status != AT_BLE_SUCCESS)
                            {
                                break;
                            }
                        }
                    }
                    /* Finally the descriptors */
                    if (is_extended_properties_defined(svc->char_list[i].char_val.properties))
                    {
                        Accumulated_handle++;
                    }
                    if (charactaristic->user_desc.len)
                    {
                        Accumulated_handle++;
                        charactaristic->user_desc.handle = Accumulated_handle;
                        user_description_define(Accumulated_handle, charactaristic->user_desc.permissions,
                                                charactaristic->user_desc.user_description, charactaristic->user_desc.len);
                    }
                    if (is_server_conf_defined(charactaristic->char_val.properties))
                    {
                        Accumulated_handle++;
                        charactaristic->server_config_desc.handle = Accumulated_handle;
                        server_conf_define(Accumulated_handle, charactaristic->server_config_desc.perm);
                    }
                    if (is_client_conf_defined(charactaristic->char_val.properties))
                    {
                        Accumulated_handle++;
                        charactaristic->client_config_desc.handle = Accumulated_handle;
                        client_conf_define(Accumulated_handle, charactaristic->client_config_desc.perm);
                    }
                    if (charactaristic->presentation_format)
                    {
                        Accumulated_handle++;
                        presentation_format_define(Accumulated_handle, charactaristic->presentation_format);
                    }
                }
        }
        while (0);
        FN_OUT(0);
        return (at_ble_status_t)status;
    }
    else
    {
        FN_OUT(0);
        return AT_BLE_INVALID_PARAM;
    }
}


static at_ble_status_t internal_at_ble_service_define(at_ble_uuid_t *uuid, at_ble_handle_t *service_handle,
        at_ble_included_service_t *included_list, uint16_t included_count, at_ble_characteristic_t *charactristic_list, uint16_t char_count, uint8_t Primary)

{
    UnionPtr uptr;
    int i = 0;
    uint16_t  start_hdl = 0, Accumulated_handle = 0;
    uint16_t temp_handle;
    uint16_t temp_perm;
    uint8_t status;
    struct gattm_svc_desc *svc_desc;
    // use the same buffer to collect with safety offset 10 bytes
    uptr.u8ptr = (uint8_t *)&interface_send_msg[20/*must be multibles of 2(Largest alignment requirement of any member ) to be sure from Alignment*/];
    svc_desc = (struct gattm_svc_desc *)uptr.u32ptr;
    TEMP_FN_IN();
    svc_desc->start_hdl = 0;
    svc_desc->task_id = TASK_GTL;
    memset(svc_desc->uuid, 0, ATT_UUID_128_LEN);
    cpy_UUID_IntoBuffer(svc_desc->uuid, uuid);
    svc_desc->perm = 0x04 | (uuid->type << 4) | (Primary << 6);
    svc_desc->nb_att = 0;
    do
    {
        /* then go through include services */
        for (i = 0; i < included_count; i++)
        {
            /*uuid_size(&included_list[i].uuid) + 4*/
            status = gattm_add_attribute_req_handler(svc_desc, included_list[i].service_handle, PERM(RD, ENABLE),
                     sizeof(include_uuid), include_uuid, &temp_handle);
        }
        /* Finally the characteristic list */
        for (i = 0; i < char_count; i++)
        {
            at_ble_characteristic_t *charactaristic = &charactristic_list[i];
            /* define "char decl" attribute */
            status = gattm_add_attribute_req_handler(svc_desc, 4 /*No meaning:
            For Characteristic Declarations this field is not used*/,
                     PERM(RD, ENABLE),
                     sizeof(char_uuid), char_uuid, &temp_handle);
            if (status != AT_BLE_SUCCESS)
            {
                break;
            }
            temp_perm = char_permissions(charactaristic->properties, charactaristic->value_permissions);
            att_permPackUUID(&temp_perm, charactaristic->uuid.type);
            /* define the attribute holding the value */
            status = gattm_add_attribute_req_handler(svc_desc, RI_FLAG(charactaristic->value_permissions) | charactaristic->value_max_len, temp_perm ,
                     at_ble_uuid_type2len(charactaristic->uuid.type), charactaristic->uuid.uuid, &charactaristic->char_val_handle);
            if (status != AT_BLE_SUCCESS)
            {
                break;
            }
            /* Finally the descriptors */
            if (is_extended_properties_defined(charactristic_list[i].properties))
            {
                uint8_t temp_buff[2] = {0x00, 0x00};
                if ((charactaristic->properties & AT_BLE_CHAR_RELIABLE_WRITE) == AT_BLE_CHAR_RELIABLE_WRITE)
                {
                    temp_buff[0] |= 0x01;
                }
                if ((charactaristic->properties & AT_BLE_CHAR_WRITEABLE_AUX) == AT_BLE_CHAR_WRITEABLE_AUX)
                {
                    temp_buff[0] |= 0x02;
                }
                gattm_add_attribute_req_handler(svc_desc, ((uint16_t)temp_buff[0]) << 8 | temp_buff[1], PERM(RD, ENABLE),
                                                sizeof(extended_props_uuid), extended_props_uuid, &temp_handle);
            }
            if (charactaristic->user_desc_len)
            {
                uint16_t perm = PERM(RD, ENABLE);
                if ((charactaristic->properties & AT_BLE_CHAR_WRITEABLE_AUX) == AT_BLE_CHAR_WRITEABLE_AUX)
                {
                    perm = att_permissions(charactaristic->user_desc_permissions, 1, 1);
                }
                else
                {
                    perm = att_permissions(charactaristic->user_desc_permissions, 1, 0);
                }
                gattm_add_attribute_req_handler(svc_desc, charactaristic->user_desc_len, RI_FLAG(charactaristic->user_desc_permissions) | perm,
                                                sizeof(user_desc_uuid), user_desc_uuid, &charactaristic->user_desc_handle);
            }
            if (is_server_conf_defined(charactristic_list[i].properties))
            {
                gattm_add_attribute_req_handler(svc_desc, 2, att_permissions(charactaristic->server_config_permissions, 1, 0),
                                                sizeof(server_conf_uuid), server_conf_uuid, &charactaristic->server_config_handle);
            }
            if (is_client_conf_defined(charactristic_list[i].properties))
            {
                gattm_add_attribute_req_handler(svc_desc, 2, att_permissions(charactaristic->client_config_permissions, 1, 1),
                                                sizeof(client_conf_uuid), client_conf_uuid, &charactaristic->client_config_handle);
            }
            if (charactristic_list[i].presentation_format)
            {
                gattm_add_attribute_req_handler(svc_desc, 7, PERM(RD, ENABLE),
                                                sizeof(presentation_format_uuid), presentation_format_uuid, &temp_handle);
            }
        }
        status = gattm_add_svc_req_handler(svc_desc);
        if (status != AT_BLE_SUCCESS)
        {
            break;
        }
        *service_handle = svc_desc->start_hdl;
        start_hdl = svc_desc->start_hdl;
        Accumulated_handle = start_hdl;
        /* define "include" attributes */
        for (i = 0; i < included_count; i++)
        {
            Accumulated_handle++;
        }
        /* define characteristics */
        for (i = 0; i < char_count; i++)
        {
            at_ble_characteristic_t *charactaristic = &charactristic_list[i];
            Accumulated_handle++;
            Accumulated_handle++;
            charactaristic->char_val_handle = Accumulated_handle;
            if (charactaristic->value_init_len > 0)
            {
#define P_DB(X,Y) (RI_FLAG(X)? Y: NULL)/*Present in DB*/
                //status = at_ble_characteristic_value_set(charactaristic->char_val_handle,
                //  charactaristic->init_value,0,charactaristic->value_init_len);
                if (RI_FLAG(charactaristic->value_permissions))
                {
                    db_addATT(charactaristic->char_val_handle,
                              UN_NULL(charactaristic->init_value), charactaristic->value_max_len, charactaristic->value_permissions);
                }
                else
                {
                    db_addATT(charactaristic->char_val_handle,
                              NULL, charactaristic->value_max_len, charactaristic->value_permissions);
                    gattm_att_set_value_req_handler(charactaristic->char_val_handle, charactaristic->value_init_len,
                                                    charactaristic->init_value);
                    if (status != AT_BLE_SUCCESS)
                    {
                        break;
                    }
                }
            }
            if (is_extended_properties_defined(charactristic_list[i].properties))
            {
                Accumulated_handle++;
            }
            if (charactaristic->user_desc_len)
            {
                Accumulated_handle++;
                charactaristic->user_desc_handle = Accumulated_handle;
                user_description_define(Accumulated_handle, charactaristic->user_desc_permissions
                                        , charactaristic->user_desc, charactaristic->user_desc_max_len);
            }
            if (is_server_conf_defined(charactaristic->properties))
            {
                Accumulated_handle++;
                charactaristic->server_config_handle = Accumulated_handle;
                server_conf_define(Accumulated_handle, charactaristic->server_config_permissions);
            }
            if (is_client_conf_defined(charactaristic->properties))
            {
                Accumulated_handle++;
                charactaristic->client_config_handle = Accumulated_handle;
                client_conf_define(Accumulated_handle, charactaristic->client_config_permissions);
            }
            if (charactaristic->presentation_format)
            {
                Accumulated_handle++;
                presentation_format_define(Accumulated_handle, charactaristic->presentation_format);
            }
        }
    }
    while (0);
    FN_OUT(0);
    //at_status = at_ble_att_error(status);
    return (at_ble_status_t)status;
}

at_ble_status_t at_ble_primary_service_define(at_ble_uuid_t *uuid, at_ble_handle_t *service_handle,
        at_ble_included_service_t *included_list, uint16_t included_count, at_ble_characteristic_t *charactristic_list, uint16_t char_count)

{
    TEMP_FN_IN();
    FN_OUT(0);
    return internal_at_ble_service_define(uuid, service_handle, included_list,
                                          included_count,   charactristic_list,  char_count,  PRIMARY_SERVICE);
}



at_ble_status_t at_ble_secondary_service_define(at_ble_uuid_t *uuid, at_ble_handle_t *service_handle,
        at_ble_included_service_t *included_list, uint16_t included_count, at_ble_characteristic_t *charactristic_list, uint16_t char_count)
{
    TEMP_FN_IN();
    FN_OUT(0);
    return internal_at_ble_service_define(uuid, service_handle, included_list,
                                          included_count,   charactristic_list,  char_count,  SECONDARY_SERVICE);
}


at_ble_status_t at_ble_characteristic_value_set(at_ble_handle_t handle,
        uint8_t *value, uint16_t len)
{
    uint8_t *data = 0;
    valueElem_t *elem = 0;
    TEMP_FN_IN();
    elem = db_searchATT(handle);
    if (elem && elem->data)
    {
        data = elem->data;
        memcpy(data, value, len);
        FN_OUT(0);
        return AT_BLE_SUCCESS;
    }
    else
    {
        uint8_t att_status = gattm_att_set_value_req_handler(handle, len, value);
        FN_OUT(0);
        return (at_ble_status_t)att_status;
    }
}


at_ble_status_t at_ble_characteristic_value_get(at_ble_handle_t handle, uint8_t *value, uint16_t *len)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    valueElem_t *elem = 0;
    TEMP_FN_IN();
    do
    {
    elem = db_searchATT(handle);
    if (elem && elem->data)
    {
            *len = elem->length;
            memcpy(value, elem->data, elem->length);
            break;
    }
    else
    {
            status = (at_ble_status_t)gattm_att_get_value_req_handler(handle, len, value);
            break;
    }
}
    while (0);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_descriptor_value_set(at_ble_handle_t handle,
        uint8_t *value, uint16_t len)
{
	TEMP_FN_IN();
    return at_ble_characteristic_value_set(handle, value,  len);
}

at_ble_status_t at_ble_descriptor_value_get(at_ble_handle_t handle, uint8_t *value, uint16_t *len)
{
	TEMP_FN_IN();
    return at_ble_characteristic_value_get(handle,  value,  len);
}


at_ble_status_t at_ble_read_authorize_reply
(at_ble_handle_t  conn_handle,
 at_ble_handle_t  attr_handle,
 bool  grant_authorization
)
{
    TEMP_FN_IN();
    if (grant_authorization == NMI_TRUE)
    {
        valueElem_t *elem = 0;
        elem = db_searchATT(attr_handle);
        if (elem && elem->data)
        {
            gattc_read_cfm(attr_handle, conn_handle, 0x00, elem->length, elem->data);
        }
    }
    else
    {
        gattc_read_cfm(attr_handle, conn_handle, 0x08, 0, 0);
    }
    FN_OUT(0);
    return AT_BLE_SUCCESS;
}


at_ble_status_t at_ble_write_authorize_reply
(at_ble_handle_t  conn_handle,
 at_ble_handle_t  attr_handle,
 bool  grant_authorization
)
{
    TEMP_FN_IN();
    if (grant_authorization == NMI_TRUE)
    {
        valueElem_t *elem = 0;
        elem = db_searchATT(attr_handle);
        if (elem)
        {
            gattc_write_cfm(attr_handle, conn_handle, 0x00);
        }
    }
    else
    {
        gattc_write_cfm(attr_handle, conn_handle, 0x08);
    }
    FN_OUT(0);
    return AT_BLE_SUCCESS;
}
at_ble_events_t gatts_handle_read_req(uint8_t *data, at_ble_characteristic_read_req_t *params)
{
    at_ble_events_t evt_num = AT_BLE_EVENT_MAX ;
    valueElem_t *elem = 0;
    TEMP_FN_IN();
    gattc_read_req_ind_parser(data, params);
    elem = db_searchATT(params->char_handle);
    if (elem->Authz_type & READ_AUTHZ)
    {
        evt_num = AT_BLE_READ_AUTHORIZE_REQUEST;
    }
    else
    {
        at_ble_read_authorize_reply(params->conn_handle, params->char_handle, 1);
    }
    FN_OUT(0);
    return evt_num;
}

at_ble_events_t gatts_handle_att_info_req(uint8_t *data, at_ble_att_info_req_t *params, uint16_t src_id)
{
    at_ble_events_t evt_num = AT_BLE_EVENT_MAX ;
    valueElem_t *elem;
    TEMP_FN_IN();
    gattc_att_info_req_parser(data, params);
    elem = db_searchATT(params->char_handle);
    if (elem)
    {
        gattc_att_info_cfm(params->char_handle, KE_IDX_GET(src_id), elem->length, 0);
    }
    else
    {
        gattc_att_info_cfm(params->char_handle, KE_IDX_GET(src_id), 1, 0);
    }
    FN_OUT(0);
    return evt_num;
}


at_ble_events_t gatts_handle_write_req(uint8_t *data, at_ble_characteristic_write_req_t *params)
{
    at_ble_events_t evt_num = AT_BLE_EVENT_MAX ;
    valueElem_t *elem = 0;
    TEMP_FN_IN();
    gattc_write_req_ind_parser(data, params);
    elem = db_searchATT(params->char_handle);
    if (elem->Authz_type & WRITE_AUTHZ)
    {
        evt_num = AT_BLE_WRITE_AUTHORIZE_REQUEST;
    }
    else
    {
        evt_num = AT_BLE_CHARACTERISTIC_CHANGED;
        if (elem->data)
        {
            memcpy(elem->data + params->offset, params->value, params->length);
        }
        else
        {
            uint16_t len = 0;
            at_ble_characteristic_value_get(params->char_handle, dummy_buffer, &len);
            memcpy(dummy_buffer + params->offset, params->value, params->length);
            at_ble_characteristic_value_set(params->char_handle, dummy_buffer, len);
        }
        at_ble_write_authorize_reply(params->conn_handle, params->char_handle, 1);
        ((at_ble_characteristic_changed_t *)params)->status = AT_BLE_SUCCESS;
    }
    FN_OUT(0);
    return evt_num;
}


at_ble_status_t at_ble_indication_send(at_ble_handle_t conn_handle, at_ble_handle_t attr_handle)
{
    valueElem_t *elem = 0;
    at_ble_status_t satus = AT_BLE_SUCCESS;
    uint8_t Val[AT_BLE_MAX_ATT_LEN];
    uint16_t Length;
    TEMP_FN_IN();
    if (u32usedBuffers < MAX_USED_BUFFERS)
    {
        elem = db_searchATT(attr_handle);
        if (elem && elem->data)
        {
            gattc_send_evt_cmd_handler(GATTC_INDICATE, attr_handle, conn_handle, elem->data, elem->length);
            satus = AT_BLE_SUCCESS;
        }
        else
        {
            gattm_att_get_value_req_handler(attr_handle, &Length, Val);
            gattc_send_evt_cmd_handler(GATTC_INDICATE, attr_handle, conn_handle, Val, Length);
            satus = AT_BLE_SUCCESS;
        }
        u32usedBuffers++;
    }
    else
    {
        satus = AT_BLE_BUSY;
    }
    FN_OUT(0);
    return satus;
}
at_ble_status_t at_ble_notification_send(at_ble_handle_t conn_handle, at_ble_handle_t attr_handle)
{
    valueElem_t *elem = 0;
    at_ble_status_t satus = AT_BLE_SUCCESS;
    uint8_t Val[AT_BLE_MAX_ATT_LEN];
    uint16_t Length;
    TEMP_FN_IN();
    if (u32usedBuffers < MAX_USED_BUFFERS)
    {
        elem = db_searchATT(attr_handle);
        if (elem && elem->data)
        {
            //conn_handle,elem->data[0]=7;
            gattc_send_evt_cmd_handler(GATTC_NOTIFY, attr_handle, conn_handle, elem->data, elem->length);
            satus = AT_BLE_SUCCESS;
        }
        else
        {
            gattm_att_get_value_req_handler(attr_handle, &Length, Val);
            gattc_send_evt_cmd_handler(GATTC_NOTIFY, attr_handle, conn_handle, Val, Length);
            satus = AT_BLE_SUCCESS;
        }
        u32usedBuffers++;
    }
    else
    {
        satus = AT_BLE_BUSY;
    }
    FN_OUT(0);
    return satus;
}

at_ble_status_t at_ble_send_service_changed_indication(at_ble_handle_t conn_handle,
        at_ble_handle_t start_handle,
        at_ble_handle_t end_handle)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    TEMP_FN_IN();
    do
    {
        if (start_handle > end_handle)
        {
            status = AT_BLE_INVALID_PARAM;
            break;
        }
        gattc_svc_changed_ind_cmd_handler(conn_handle, start_handle, end_handle);
    }
    while (0);
    ASSERT_PRINT_ERR(AT_BLE_SUCCESS != status, "Status : 0x%02X\n", status);
    FN_OUT(status);
    return status;
}
