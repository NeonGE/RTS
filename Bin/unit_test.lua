--resultado = HelloLuaWorld(8,5);
--print("El Resultado fue:", resultado);
local user = os.getenv("USERNAME");
print(user);

local x = os.clock();
local s = 0;
for i=1,10000000 do
	s = s + i
end
print("Wut!");
print(string.format("Tiempo Estimado: %.2f\n", os.clock() - x));
print("Wut Wut!");
