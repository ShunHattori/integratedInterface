#pragma once

#include "Arduino.h"
#include "../lib/ComponentPropaties.hpp"
#include "../lib/RobotFlagList.hpp"

class RemoteEmergency
{
private:
    FlagSet &flag_set;
    uint8_t comm_data[20];

    struct IM920CommPropaties
    {
        const int dataFrameLenght = 14, //通信データフレームの長さ(bytes)
            uniqueIDIndex = 0,          //データフレーム内固有IDのインデックス(NodeIDは無視)
            RSSIIndex = 5,              //データフレーム内RSSI値のインデックス
            switchDataIndex = 8,        //データフレーム内接点情報のインデックス
            controllerIndex = 11,       //コントローラのIDのインデックス（直後にスイッチ入力）
            timeoutInMicros = 1000,     //タイムアウトまでの猶予期間
            ASCIITableNumberOffset = 48;
    } IM920_comm_props;

    struct EmergencyData
    {
        int RSSI;
        int allContactState;
        bool ownContactState;
    } emergency_data;

    uint8_t readDatas();
    uint8_t parseDatas();
    uint8_t applyContactState();

    /*
   * データ読み取り可能時1、不可時0を返す
   */
    bool isDataAvailable();

    /*
   * データ到着時に1、タイムアウト時に0をリターン
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
    readDatas();
    parseDatas();
    applyContactState();
    return 0;
}

inline uint8_t RemoteEmergency::readDatas()
{
    if (!isDataAvailable())
        return 1;

    uint8_t raw_data[20], index_num = 0;
    for (index_num = 0; index_num < 4; index_num++)
    {
        if (!waitDataArrival())
            return 2;
        raw_data[index_num] = EMERGENCY_PROPS.IM920.port.read();
        if (raw_data[index_num] != EMERGENCY_PROPS.IM920.unique_id[index_num])
            return 3;
    }
    for (; index_num < IM920_comm_props.dataFrameLenght; index_num++)
    {
        if (!waitDataArrival())
            return 4;
        raw_data[index_num] = EMERGENCY_PROPS.IM920.port.read();
    }
    for (int i = 0; i < 20; i++)
    {
        comm_data[i] = raw_data[i];
    }
    return 0;
}

inline uint8_t RemoteEmergency::parseDatas()
{
    uint8_t sw_num_tens_place = (comm_data[IM920_comm_props.switchDataIndex] - IM920_comm_props.ASCIITableNumberOffset) * 10,
            sw_num_ones_place = (comm_data[IM920_comm_props.switchDataIndex + 1] - IM920_comm_props.ASCIITableNumberOffset);
    emergency_data.allContactState = sw_num_tens_place + sw_num_ones_place;
    emergency_data.ownContactState = (emergency_data.allContactState & EMERGENCY_PROPS.own_signal_bit) == 0 ? 0 : 1;

    char RSSI[2]{
        (char)comm_data[IM920_comm_props.RSSIIndex],
        (char)comm_data[IM920_comm_props.RSSIIndex + 1],
    };
    emergency_data.RSSI = strtol(RSSI, NULL, 16);
    return 5;
}

inline uint8_t RemoteEmergency::applyContactState()
{
    digitalWrite(EMERGENCY_PROPS.emergency_signal_pin, emergency_data.ownContactState);
    flag_set.sw_state_emergency = emergency_data.ownContactState;
    return 0;
}

inline bool RemoteEmergency::isDataAvailable()
{
    if (EMERGENCY_PROPS.IM920.port.available())
        return 1;
    return 0;
}

inline bool RemoteEmergency::waitDataArrival()
{
    long long int process_started_period = micros();
    while (1)
    {
        if ((micros() - process_started_period) > IM920_comm_props.timeoutInMicros)
            return 0;
        if (isDataAvailable())
            return 1;
    }
}
