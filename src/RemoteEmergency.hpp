#pragma once
#include "Arduino.h"
#include "ComponentPropaties.hpp"
#include "RobotFlagList.hpp"

class RemoteEmergency
{
    using packet = uint8_t;
    using ll = long long int;

public:
    RemoteEmergency() = delete;
    RemoteEmergency(FlagSet &flag_set_)
        : flag_set(flag_set_) {}

    virtual ~RemoteEmergency() {}

    uint8_t work()
    {
        if (!isDataAvailable())
            return 1;
        packet raw_data[20], index_num = 0;
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

        uint8_t sw_num_tens_place =
                    (raw_data[IM920_comm_props.switchDataIndex] - IM920_comm_props.ASCIITableNumberOffset) * 10,
                sw_num_ones_place =
                    (raw_data[IM920_comm_props.switchDataIndex + 1] - IM920_comm_props.ASCIITableNumberOffset);
        emergency_data.allContactState = sw_num_tens_place + sw_num_ones_place;
        emergency_data.ownContactState = (emergency_data.allContactState & EMERGENCY_PROPS.own_signal_bit) == 0 ? 0 : 1;

        char RSSI[2]{
            (char)raw_data[IM920_comm_props.RSSIIndex],
            (char)raw_data[IM920_comm_props.RSSIIndex + 1],
        };
        emergency_data.RSSI = strtol(RSSI, NULL, 16);

        return 5;
    }

    /*
     * データ読み取り可能時1、不可時0を返す
    */
    bool isDataAvailable()
    {
        if (EMERGENCY_PROPS.IM920.port.available())
            return 1;
        return 0;
    }

    /*
     * データ到着時に1、タイムアウト時に0をリターン
    */
    bool waitDataArrival()
    {
        ll process_started_period = micros();
        while (1)
        {
            if ((micros() - process_started_period) > IM920_comm_props.timeoutInMicros)
                return 0;
            if (isDataAvailable())
                return 1;
        }
    }

private:
    FlagSet flag_set;

    struct IM920CommPropaties
    {
        const int
            dataFrameLenght = 14,   //通信データフレームの長さ(bytes)
            uniqueIDIndex = 0,      //データフレーム内固有IDのインデックス(NodeIDは無視)
            RSSIIndex = 5,          //データフレーム内RSSI値のインデックス
            switchDataIndex = 8,    //データフレーム内接点情報のインデックス
            controllerIndex = 11,   //コントローラのIDのインデックス（直後にスイッチ入力）
            timeoutInMicros = 1000, //タイムアウトまでの猶予期間
            ASCIITableNumberOffset = 48;
    } IM920_comm_props;

    struct EmergencyData
    {
        int RSSI;
        int allContactState;
        bool ownContactState;
    } emergency_data;
};
