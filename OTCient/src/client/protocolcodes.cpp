/*
 * Copyright (c) 2010-2025 OTClient <https://github.com/edubart/otclient>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "protocolcodes.h"

namespace Proto
{
    stdext::map<uint8_t, uint8_t > messageModesMap;

    void buildMessageModesMap(const int version)
    {
        messageModesMap.clear();

        if (version >= 1094) {
            messageModesMap[Otc::MessageMana] = 43;
        }

        if (version >= 1055) { // might be 1054
            messageModesMap[Otc::MessageNone] = 0;
            messageModesMap[Otc::MessageSay] = 1;
            messageModesMap[Otc::MessageWhisper] = 2;
            messageModesMap[Otc::MessageYell] = 3;
            messageModesMap[Otc::MessagePrivateFrom] = 4;
            messageModesMap[Otc::MessagePrivateTo] = 5;
            messageModesMap[Otc::MessageChannelManagement] = 6;
            messageModesMap[Otc::MessageChannel] = 7;
            messageModesMap[Otc::MessageChannelHighlight] = 8;
            messageModesMap[Otc::MessageSpell] = 9;
            messageModesMap[Otc::MessageNpcFromStartBlock] = 10;
            messageModesMap[Otc::MessageNpcFrom] = 11;
            messageModesMap[Otc::MessageNpcTo] = 12;
            messageModesMap[Otc::MessageGamemasterBroadcast] = 13;
            messageModesMap[Otc::MessageGamemasterChannel] = 14;
            messageModesMap[Otc::MessageGamemasterPrivateFrom] = 15;
            messageModesMap[Otc::MessageGamemasterPrivateTo] = 16;
            messageModesMap[Otc::MessageLogin] = 17;
            messageModesMap[Otc::MessageWarning] = 18; // Admin
            messageModesMap[Otc::MessageGame] = 19;
            messageModesMap[Otc::MessageGameHighlight] = 20;
            messageModesMap[Otc::MessageFailure] = 21;
            messageModesMap[Otc::MessageLook] = 22;
            messageModesMap[Otc::MessageDamageDealed] = 23;
            messageModesMap[Otc::MessageDamageReceived] = 24;
            messageModesMap[Otc::MessageHeal] = 25;
            messageModesMap[Otc::MessageExp] = 26;
            messageModesMap[Otc::MessageDamageOthers] = 27;
            messageModesMap[Otc::MessageHealOthers] = 28;
            messageModesMap[Otc::MessageExpOthers] = 29;
            messageModesMap[Otc::MessageStatus] = 30;
            messageModesMap[Otc::MessageLoot] = 31;
            messageModesMap[Otc::MessageTradeNpc] = 32;
            messageModesMap[Otc::MessageGuild] = 33;
            messageModesMap[Otc::MessagePartyManagement] = 34;
            messageModesMap[Otc::MessageParty] = 35;
            messageModesMap[Otc::MessageBarkLow] = 36;
            messageModesMap[Otc::MessageBarkLoud] = 37;
            messageModesMap[Otc::MessageReport] = 38;
            messageModesMap[Otc::MessageHotkeyUse] = 39;
            messageModesMap[Otc::MessageTutorialHint] = 40;
            messageModesMap[Otc::MessageThankyou] = 41;
            messageModesMap[Otc::MessageMarket] = 42;
            messageModesMap[Otc::MessageMana] = 43;
            messageModesMap[Otc::MessageBeyondLast] = 44;
            messageModesMap[Otc::MessageAttention] = 48;
            messageModesMap[Otc::MessageBoostedCreature] = 49;
            messageModesMap[Otc::MessageOfflineTrainning] = 50;
            messageModesMap[Otc::MessageTransaction] = 51;
            messageModesMap[Otc::MessagePotion] = 52;
        } else if (version >= 1041) {
            messageModesMap[Otc::MessageNone] = 0;
            messageModesMap[Otc::MessageSay] = 1;
            messageModesMap[Otc::MessageWhisper] = 2;
            messageModesMap[Otc::MessageYell] = 3;
            messageModesMap[Otc::MessagePrivateFrom] = 4;
            messageModesMap[Otc::MessagePrivateTo] = 5;
            messageModesMap[Otc::MessageChannelManagement] = 6;
            messageModesMap[Otc::MessageChannel] = 7;
            messageModesMap[Otc::MessageChannelHighlight] = 8;
            messageModesMap[Otc::MessageSpell] = 9;
            messageModesMap[Otc::MessageNpcFromStartBlock] = 10;
            messageModesMap[Otc::MessageNpcFrom] = 11;
            messageModesMap[Otc::MessageNpcTo] = 12;
            messageModesMap[Otc::MessageGamemasterBroadcast] = 13;
            messageModesMap[Otc::MessageGamemasterChannel] = 14;
            messageModesMap[Otc::MessageGamemasterPrivateFrom] = 15;
            messageModesMap[Otc::MessageGamemasterPrivateTo] = 16;
            messageModesMap[Otc::MessageLogin] = 17;
            messageModesMap[Otc::MessageWarning] = 18; // Admin
            messageModesMap[Otc::MessageGame] = 19;
            messageModesMap[Otc::MessageFailure] = 20;
            messageModesMap[Otc::MessageLook] = 21;
            messageModesMap[Otc::MessageDamageDealed] = 22;
            messageModesMap[Otc::MessageDamageReceived] = 23;
            messageModesMap[Otc::MessageHeal] = 24;
            messageModesMap[Otc::MessageExp] = 25;
            messageModesMap[Otc::MessageDamageOthers] = 26;
            messageModesMap[Otc::MessageHealOthers] = 27;
            messageModesMap[Otc::MessageExpOthers] = 28;
            messageModesMap[Otc::MessageStatus] = 29;
            messageModesMap[Otc::MessageLoot] = 30;
            messageModesMap[Otc::MessageTradeNpc] = 31;
            messageModesMap[Otc::MessageGuild] = 32;
            messageModesMap[Otc::MessagePartyManagement] = 33;
            messageModesMap[Otc::MessageParty] = 34;
            messageModesMap[Otc::MessageBarkLow] = 35;
            messageModesMap[Otc::MessageBarkLoud] = 36;
            messageModesMap[Otc::MessageReport] = 37;
            messageModesMap[Otc::MessageHotkeyUse] = 38;
            messageModesMap[Otc::MessageTutorialHint] = 49;
            messageModesMap[Otc::MessageThankyou] = 40;
            messageModesMap[Otc::MessageMarket] = 41;
        } else if (version >= 1036) {
            for (int i = Otc::MessageNone; i <= Otc::MessageBeyondLast; ++i) {
                if (i >= Otc::MessageNpcTo)
                    messageModesMap[i] = i + 1;
                else
                    messageModesMap[i] = i;
            }
        } else if (version >= 900) {
            for (int i = Otc::MessageNone; i <= Otc::MessageBeyondLast; ++i)
                messageModesMap[i] = i;
        } else if (version >= 861) {
            messageModesMap[Otc::MessageNone] = 0;
            messageModesMap[Otc::MessageSay] = 1;
            messageModesMap[Otc::MessageWhisper] = 2;
            messageModesMap[Otc::MessageYell] = 3;
            messageModesMap[Otc::MessageNpcTo] = 4;
            messageModesMap[Otc::MessageNpcFrom] = 5;
            messageModesMap[Otc::MessagePrivateFrom] = 6;
            messageModesMap[Otc::MessagePrivateTo] = 6;
            messageModesMap[Otc::MessageChannel] = 7;
            messageModesMap[Otc::MessageChannelManagement] = 8;
            messageModesMap[Otc::MessageGamemasterBroadcast] = 9;
            messageModesMap[Otc::MessageGamemasterChannel] = 10;
            messageModesMap[Otc::MessageGamemasterPrivateFrom] = 11;
            messageModesMap[Otc::MessageGamemasterPrivateTo] = 11;
            messageModesMap[Otc::MessageChannelHighlight] = 12;
            messageModesMap[Otc::MessageMonsterSay] = 13;
            messageModesMap[Otc::MessageMonsterYell] = 14;
            messageModesMap[Otc::MessageWarning] = 15;
            messageModesMap[Otc::MessageGame] = 16;
            messageModesMap[Otc::MessageLogin] = 17;
            messageModesMap[Otc::MessageStatus] = 18;
            messageModesMap[Otc::MessageLook] = 19;
        messageModesMap[Otc::MessageFailure] = 20;
        messageModesMap[Otc::MessageBlue] = 21;
        messageModesMap[Otc::MessageRed] = 22;
        messageModesMap[Otc::MessageDamageDealed] = 23;
        messageModesMap[Otc::MessageDamageReceived] = 24;
        messageModesMap[Otc::MessageHeal] = 25;
        messageModesMap[Otc::MessageExp] = 26;
        messageModesMap[Otc::MessageDamageOthers] = 27;
        messageModesMap[Otc::MessageHealOthers] = 28;
        messageModesMap[Otc::MessageExpOthers] = 29;
    } else if (version >= 840) {
            messageModesMap[Otc::MessageNone] = 0;
            messageModesMap[Otc::MessageSay] = 1;
            messageModesMap[Otc::MessageWhisper] = 2;
            messageModesMap[Otc::MessageYell] = 3;
            messageModesMap[Otc::MessageNpcTo] = 4;
            messageModesMap[Otc::MessageNpcFromStartBlock] = 5;
            messageModesMap[Otc::MessagePrivateFrom] = 6;
            messageModesMap[Otc::MessagePrivateTo] = 6;
            messageModesMap[Otc::MessageChannel] = 7;
            messageModesMap[Otc::MessageChannelManagement] = 8;
            messageModesMap[Otc::MessageRVRChannel] = 9;
            messageModesMap[Otc::MessageRVRAnswer] = 10;
            messageModesMap[Otc::MessageRVRContinue] = 11;
            messageModesMap[Otc::MessageGamemasterBroadcast] = 12;
            messageModesMap[Otc::MessageGamemasterChannel] = 13;
            messageModesMap[Otc::MessageGamemasterPrivateFrom] = 14;
            messageModesMap[Otc::MessageGamemasterPrivateTo] = 14;
            messageModesMap[Otc::MessageChannelHighlight] = 15;
            // 16, 17 ??
            messageModesMap[Otc::MessageRed] = 18;
            messageModesMap[Otc::MessageMonsterSay] = 19;
            messageModesMap[Otc::MessageMonsterYell] = 20;
            messageModesMap[Otc::MessageWarning] = 21;
            messageModesMap[Otc::MessageGame] = 22;
            messageModesMap[Otc::MessageLogin] = 23;
            messageModesMap[Otc::MessageStatus] = 24;
            messageModesMap[Otc::MessageLook] = 25;
            messageModesMap[Otc::MessageFailure] = 26;
            messageModesMap[Otc::MessageBlue] = 27;
            messageModesMap[Otc::MessageDamageDealed] = 21;
            messageModesMap[Otc::MessageDamageReceived] = 22;
            messageModesMap[Otc::MessageHeal] = 23;
            messageModesMap[Otc::MessageExp] = 24;
            messageModesMap[Otc::MessageDamageOthers] = 25;
            messageModesMap[Otc::MessageHealOthers] = 26;
            messageModesMap[Otc::MessageExpOthers] = 27;
        } else if (version >= 740) {
            messageModesMap[Otc::MessageNone] = 0;
            messageModesMap[Otc::MessageSay] = 1;
            messageModesMap[Otc::MessageWhisper] = 2;
            messageModesMap[Otc::MessageYell] = 3;
            messageModesMap[Otc::MessagePrivateFrom] = 4;
            messageModesMap[Otc::MessagePrivateTo] = 4;
            messageModesMap[Otc::MessageChannel] = 5;
            messageModesMap[Otc::MessageRVRChannel] = 6;
            messageModesMap[Otc::MessageRVRAnswer] = 7;
            messageModesMap[Otc::MessageRVRContinue] = 8;
            messageModesMap[Otc::MessageGamemasterBroadcast] = 9;
            messageModesMap[Otc::MessageGamemasterChannel] = 10;
            messageModesMap[Otc::MessageGamemasterPrivateFrom] = 11;
            messageModesMap[Otc::MessageGamemasterPrivateTo] = 11;
            messageModesMap[Otc::MessageChannelHighlight] = 12;
            // 13, 14, 15 ??
            messageModesMap[Otc::MessageMonsterSay] = 16;
            messageModesMap[Otc::MessageMonsterYell] = 17;
            messageModesMap[Otc::MessageWarning] = 18;
            messageModesMap[Otc::MessageGame] = 19;
            messageModesMap[Otc::MessageLogin] = 20;
            messageModesMap[Otc::MessageStatus] = 21;
            messageModesMap[Otc::MessageLook] = 22;
            messageModesMap[Otc::MessageFailure] = 23;
            messageModesMap[Otc::MessageBlue] = 24;
            messageModesMap[Otc::MessageRed] = 25;
            messageModesMap[Otc::MessageDamageDealed] = 21;
            messageModesMap[Otc::MessageDamageReceived] = 22;
            messageModesMap[Otc::MessageHeal] = 23;
            messageModesMap[Otc::MessageExp] = 24;
            messageModesMap[Otc::MessageDamageOthers] = 25;
            messageModesMap[Otc::MessageHealOthers] = 26;
            messageModesMap[Otc::MessageExpOthers] = 27;
        }
    }

    Otc::MessageMode translateMessageModeFromServer(const uint8_t mode)
    {
        const auto it = std::ranges::find_if(messageModesMap, [=](const std::pair<uint8_t, uint8_t >& p) { return p.second == mode; });
        if (it != messageModesMap.end())
            return static_cast<Otc::MessageMode>(it->first);
        return Otc::MessageInvalid;
    }

    uint8_t translateMessageModeToServer(const Otc::MessageMode mode)
    {
        if (mode >= Otc::LastMessage)
            return Otc::MessageInvalid;
        if (const auto it = messageModesMap.find(mode); it != messageModesMap.end())
            return it->second;
        return Otc::MessageInvalid;
    }
}