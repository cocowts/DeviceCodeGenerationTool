
local function bin2hex(s)
    s=string.gsub(s,"(.)",function (x) return string.format("%02X",string.byte(x)) end)
    return s
end

function conv(rawNum)
    if #rawNum ~= 18 or string.sub(rawNum, 0, 4) ~= "0922" then   
        return ""
    end
    return bin2hex(string.sub(rawNum, 6, -1))
end
