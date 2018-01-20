
function hello()
    if compteur == nil then
        cprintln("once compteur")
        compteur = 0
    end
    cprintln("hello" .. compteur)
    compteur = compteur + 1
end
