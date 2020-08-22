#pragma once

#include "Arduino.h"
#include "../lib/ComponentPropaties.hpp"
#include "../lib/RobotFlagList.hpp"
#include "../lib/ErrorLists.hpp"

/*
 * IM920 Data Frame:
 * 00,0002,9C:10,23,1
 * Node ID, UniqueID, RSSI Value: SWdigit4 SWdigit3, SWdigit2 SWdigit1 , machineNum
 * indexCouter counts index from the front of UniqueID as 0
 * index : 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14
 * datas : 0 0 0 0 , 9 C : 1 0  ,  2  3  ,  1
*/

class RemoteEmergency
{
private:
    FlagSet &flag_set;
    uint8_t comm_data[64];

    struct IM920CommPropaties
    {
        const int dataFrameLenght = 15, //通信データフレームの長さ(bytes)
            uniqueIDIndex = 0,          //データフレーム内固有IDのインデックス(NodeIDは無視)
            RSSIIndex = 5,              //データフレーム内RSSI値のインデックス
            switchDataIndex = 8,        //データフレーム内接点情報のインデックス
            targetMachineIndex = 14,    //操縦用スイッチ接点の送信先ロボット番号のインデックス
            timeoutInMicros = 1000,     //タイムアウトまでの猶予期間
            ASCIITableNumberOffset = 48;
    } IM920_comm_props;

    struct EmergencyData
    {
        uint16_t RSSI;
        uint16_t allContactState;
        bool ownContactState;
    } emergency_data;

    uint8_t readDatas();
    uint8_t parseDatas();
    uint8_t applyContactState();
    uint16_t power(uint16_t, uint16_t);

    /*
   * データ読み取り可能時none、不可時not_readyを返す
   */
    bool isDataAvailable();

    /*
   * データ到着時にnone、タイムアウト時にtimeoutをリターン
   */
    bool waitDataArrival();

public:
    RemoteEmergency() = delete;
    RemoteEmergency(FlagSet &);
    virtual ~RemoteEmergency(){};

    uint8_t work();
};

inline RemoteEmergency::RemoteEmergency(FlagSet &flag_set_) : flag_set(flag_set_)
{
    EMERGENCY_PROPS.IM920.port.begin(EMERGENCY_PROPS.IM920.baud);
};

inline uint8_t RemoteEmergency::work()
{
    if (readDatas())
        return error::auth_failed;
    if (parseDatas())
        return error::auth_failed;
    if (applyContactState())
        return error::auth_failed;
    return error::none;
}

inline uint8_t RemoteEmergency::readDatas()
{
    if (isDataAvailable())
        return error::not_ready;

    uint8_t raw_data[IM920_comm_props.dataFrameLenght], index_num = 0;
    for (index_num = 0; index_num < 4; index_num++)
    {
        if (!waitDataArrival())
            return error::timeout;
        raw_data[index_num] = EMERGENCY_PROPS.IM920.port.read();
        if (raw_data[index_num] != EMERGENCY_PROPS.IM920.unique_id[index_num])
            return error::discrepancy;
    }
    for (; index_num < IM920_comm_props.dataFrameLenght; index_num++)
    {
        if (!waitDataArrival())
            return error::timeout;
        raw_data[index_num] = EMERGENCY_PROPS.IM920.port.read();
    }
    for (int i = 0; i < IM920_comm_props.dataFrameLenght; i++)
    {
        comm_data[i] = raw_data[i];
    }
    return error::none;
}

inline uint8_t RemoteEmergency::parseDatas()
{
    uint8_t sw_state_digit[4];
    //コンマが挿入されるためcomm_data配列の参照が[2]を飛ばす形になる
    sw_state_digit[0] = (comm_data[IM920_comm_props.switchDataIndex + 0] - IM920_comm_props.ASCIITableNumberOffset);
    sw_state_digit[1] = (comm_data[IM920_comm_props.switchDataIndex + 1] - IM920_comm_props.ASCIITableNumberOffset);
    sw_state_digit[2] = (comm_data[IM920_comm_props.switchDataIndex + 3] - IM920_comm_props.ASCIITableNumberOffset);
    sw_state_digit[3] = (comm_data[IM920_comm_props.switchDataIndex + 4] - IM920_comm_props.ASCIITableNumberOffset);

    for (uint8_t i = 0; i < 4; i++)
    {
        emergency_data.allContactState += sw_state_digit[i] * power(10, i);
    }
    emergency_data.ownContactState = (emergency_data.allContactState & EMERGENCY_PROPS.own_signal_bit) != 0 ? true : false;

    if (robot_ID == comm_data[IM920_comm_props.targetMachineIndex])
    {
        flag_set.is_controller_targeted = true;
        flag_set.sw_state_phase1 = ((emergency_data.allContactState & 0b0000001000000000) != 0) ? true : false;
        flag_set.sw_state_phase2 = ((emergency_data.allContactState & 0b0000000100000000) != 0) ? true : false;
        flag_set.sw_state_phase3 = ((emergency_data.allContactState & 0b0000000010000000) != 0) ? true : false;
        flag_set.sw_state_phase4 = ((emergency_data.allContactState & 0b0000000001000000) != 0) ? true : false;
    }
    else
    {
        flag_set.is_controller_targeted = false;
    }

    char RSSI[2]{
        (char)comm_data[IM920_comm_props.RSSIIndex],
        (char)comm_data[IM920_comm_props.RSSIIndex + 1],
    };
    emergency_data.RSSI = strtol(RSSI, NULL, 16);
    return error::none;
}

inline uint8_t RemoteEmergency::applyContactState()
{
    digitalWrite(EMERGENCY_PROPS.emergency_signal_pin, emergency_data.ownContactState);
    flag_set.sw_state_emergency = emergency_data.ownContactState;
    return error::none;
}

inline bool RemoteEmergency::isDataAvailable()
{
    if (EMERGENCY_PROPS.IM920.port.available())
        return error::none;
    return error::not_ready;
}

inline bool RemoteEmergency::waitDataArrival()
{
    long long int process_started_period = micros();
    while (1)
    {
        if ((micros() - process_started_period) > IM920_comm_props.timeoutInMicros)
            return error::timeout;
        if (isDataAvailable())
            return error::none;
    }
}

inline uint16_t RemoteEmergency::power(uint16_t base, uint16_t times)
{
    uint32_t result = 1;
    for (uint8_t i = 0; i < times; i++)
    {
        result *= base;
    }
    return result;
}