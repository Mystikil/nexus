local event = Event()

function event.onInventoryUpdate(self, item, slot, equip)
    if not item then
        return
    end
    for id, _ in pairs(CustomAttributes.attributes) do
        local value = item:getCustomAttribute(id)
        if value then
            local current = self:getCustomAttribute(id)
            if equip then
                self:setCustomAttribute(id, current + value)
                if id == 18 then
                    self:setCapacity(self:getCapacity() + value * 10)
                elseif id == 28 and self:getCustomAttribute(28) > 0 then
                    self:setLight(215, 7)
                end
            else
                self:setCustomAttribute(id, current - value)
                if id == 18 then
                    self:setCapacity(self:getCapacity() - value * 10)
                elseif id == 28 and self:getCustomAttribute(28) <= 0 then
                    self:setLight(0, 0)
                end
            end
        end
    end
end

event:register()
