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

#ifdef FRAMEWORK_EDITOR
#include "game.h"
#include "thingtypemanager.h"

#include <framework/core/binarytree.h>
#include <framework/core/filestream.h>

void ItemType::unserialize(const BinaryTreePtr& node)
{
    m_null = false;

    m_category = static_cast<ItemCategory>(node->getU8());

    node->getU32(); // flags

    static uint16_t lastId = 99;
    while (node->canRead()) {
        const uint8_t attr = node->getU8();
        if (attr == 0 || attr == 0xFF)
            break;

        const uint16_t len = node->getU16();
        switch (attr) {
            case ItemTypeAttrServerId:
            {
                uint16_t serverId = node->getU16();
                if (g_game.getClientVersion() < 960) {
                    if (serverId > 20000 && serverId < 20100) {
                        serverId -= 20000;
                    } else if (lastId > 99 && lastId != serverId - 1) {
                        while (lastId != serverId - 1) {
                            const auto& tmp = std::make_shared<ItemType>();
                            tmp->setServerId(++lastId);
                            g_things.addItemType(tmp);
                        }
                    }
                } else {
                    if (serverId > 30000 && serverId < 30100) {
                        serverId -= 30000;
                    } else if (lastId > 99 && lastId != serverId - 1) {
                        while (lastId != serverId - 1) {
                            const auto& tmp = std::make_shared<ItemType>();
                            tmp->setServerId(++lastId);
                            g_things.addItemType(tmp);
                        }
                    }
                }
                setServerId(serverId);
                lastId = serverId;
                break;
            }
            case ItemTypeAttrClientId:
                setClientId(node->getU16());
                break;

            case ItemTypeAttrName:
                setName(node->getString(len));
                break;

            case ItemTypeAttrWritable:
                m_writable = true;
                break;

            default:
                node->skip(len); // skip attribute
                break;
        }
    }
}

#endif