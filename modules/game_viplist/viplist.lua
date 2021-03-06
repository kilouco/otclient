vipWindow = nil
vipButton = nil
addVipWindow = nil

function init()
  connect(g_game, { onGameEnd = clear,
                    onAddVip = onAddVip,
                    onVipStateChange = onVipStateChange })


  g_keyboard.bindKeyDown('Ctrl+P', toggle)

  vipWindow = g_ui.loadUI('viplist.otui', modules.game_interface.getRightPanel())
  vipWindow.onClose = onMiniWindowClose
  vipButton = TopMenu.addRightGameToggleButton('vipListButton', tr('VIP list') .. ' (Ctrl+P)', 'viplist.png', toggle)
  vipButton:setOn(true)

  refresh()
end

function terminate()
  g_keyboard.unbindKeyDown('Ctrl+P')
  disconnect(g_game, { onGameEnd = clear,
                       onAddVip = onAddVip,
                       onVipStateChange = onVipStateChange })

  vipWindow:destroy()
  vipButton:destroy()
end

function refresh()
  clear()
  for id,vip in pairs(g_game.getVips()) do
    onAddVip(id, unpack(vip))
  end
end

function clear()
  local vipList = vipWindow:getChildById('contentsPanel')
  vipList:destroyChildren()
end

function toggle()
  if vipButton:isOn() then
    vipWindow:close()
    vipButton:setOn(false)
  else
    vipWindow:open()
    vipButton:setOn(true)
  end
end

function onMiniWindowClose()
  vipButton:setOn(false)
end

function createAddWindow()
  addVipWindow = g_ui.displayUI('addvip.otui')
end

function destroyAddWindow()
  addVipWindow:destroy()
  addVipWindow = nil
end

function addVip()
  g_game.addVip(addVipWindow:getChildById('name'):getText())
  destroyAddWindow()
end

function onAddVip(id, name, online)
  local vipList = vipWindow:getChildById('contentsPanel')

  local label = g_ui.createWidget('VipListLabel')
  label.onMousePress = onVipListLabelMousePress
  label:setId('vip' .. id)
  label:setText(name)

  if online then
    label:setColor('#00ff00')
  else
    label:setColor('#ff0000')
  end

  label.vipOnline = online

  label:setPhantom(false)
  connect(label, { onDoubleClick = function () g_game.openPrivateChannel(label:getText()) return true end } )

  local nameLower = name:lower()
  local childrenCount = vipList:getChildCount()

  for i=1,childrenCount do
    local child = vipList:getChildByIndex(i)
    if online and not child.vipOnline then
      vipList:insertChild(i, label)
      return
    end

    if (not online and not child.vipOnline) or (online and child.vipOnline) then
      local childText = child:getText():lower()
      local length = math.min(childText:len(), nameLower:len())

      for j=1,length do
        if nameLower:byte(j) < childText:byte(j) then
          vipList:insertChild(i, label)
          return
        elseif nameLower:byte(j) > childText:byte(j) then
          break
        end
      end
    end
  end

  vipList:insertChild(childrenCount+1, label)
end

function onVipStateChange(id, online)
  local vipList = vipWindow:getChildById('contentsPanel')
  local label = vipList:getChildById('vip' .. id)
  local text = label:getText()
  label:destroy()

  onAddVip(id, text, online)
end

function onVipListMousePress(widget, mousePos, mouseButton)
  if mouseButton ~= MouseRightButton then return end

  local vipList = vipWindow:getChildById('contentsPanel')

  local menu = g_ui.createWidget('PopupMenu')
  menu:addOption(tr('Add new VIP'), function() createAddWindow() end)
  menu:display(mousePos)

  return true
end

function onVipListLabelMousePress(widget, mousePos, mouseButton)
  if mouseButton ~= MouseRightButton then return end

  local vipList = vipWindow:getChildById('contentsPanel')

  local menu = g_ui.createWidget('PopupMenu')
  menu:addOption(tr('Add new VIP'), function() createAddWindow() end)
  menu:addOption(tr('Remove %s', widget:getText()), function() if widget then g_game.removeVip(widget:getId():sub(4)) vipList:removeChild(widget) end end)
  menu:addSeparator()
  menu:addOption(tr('Copy Name'), function() g_window.setClipboardText(widget:getText()) end)
  menu:display(mousePos)

  return true
end
