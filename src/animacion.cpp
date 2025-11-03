#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>
#include <thread>
#include <chrono>
#include <iostream>
#include <cmath> // Necesario para std::sin y std::abs
// --- Archivos locales del proyecto (Deben estar en include/) ---
#include <Dibujo.hpp>
#include <GestorDibujos.hpp>
#include <Corral.hpp> 

using namespace ftxui;

// --- DIBUJOS ASCII PARA LA TEMÁTICA SPIDERMAN ---

// Spiderman balanceándose (el dibujo se puede mejorar, pero funciona para el ejemplo)
const std::vector<std::u32string> spiderman_swing = {
    U"    \\O/    ", // Manos extendidas
    U"     |     ", // Cuerpo
    U"    / \\    ", // Piernas
    U"   /   \\   "  // La 'tela' o efecto de movimiento
};

// Rascacielos A (con ventanas cuadradas)
const std::vector<std::u32string> rascacielos_a = {
    U" [][][] ",
    U" [][][] ",
    U" [][][] ",
    U" [][][] ",
    U" [][][] ",
    U" [][][] ",
    U" [][][] ",
    U" [][][] "
};

// Rascacielos B (con ventanas circulares o diferentes)
const std::vector<std::u32string> rascacielos_b = {
    U" |||||||",
    U" |||O|||",
    U" |||||||",
    U" |||O|||",
    U" |||||||",
    U" |||O|||",
    U" |||||||",
    U" |||O|||"
};


int main()
{
    // Crear la ventana (Dimensiones ajustadas a la ciudad)
    auto screen = Screen::Create(
        Dimension::Fixed(100), // Ancho 
        Dimension::Fixed(30)); // Alto

    // El 'Corral' actúa como el fondo (el cielo)
    Corral cielo(0, 0, 100, 30, ftxui::Color::Black); 
    
    GestorDibujos gestor;

    // --- CONFIGURACIÓN DEL FONDO (RASCACIELOS) ---
    
    // Rascacielos en el fondo (Dibujados primero)
    gestor.Agregar(Dibujo(5, 10, rascacielos_a, ftxui::Color::DarkGray));
    gestor.Agregar(Dibujo(25, 12, rascacielos_b, ftxui::Color::Gray));
    gestor.Agregar(Dibujo(45, 8, rascacielos_a, ftxui::Color::DarkGray));
    gestor.Agregar(Dibujo(65, 15, rascacielos_b, ftxui::Color::Gray));
    gestor.Agregar(Dibujo(85, 10, rascacielos_a, ftxui::Color::DarkGray));
    

    // --- AGREGAR SPIDERMAN ---
    // Spiderman se dibuja sobre los rascacielos
    gestor.Agregar(Dibujo(45, 10, spiderman_swing, ftxui::Color::Red)); 
    
    // El índice de Spiderman en el vector de dibujos es importante para manipularlo
    const int indice_spiderman = 5; 

    int frames = 0; // Contador de frames para la animación
    while (true) // Bucle infinito para animación continua
    {
        // 1. Limpiar la pantalla en cada frame
        screen.Clear();
        
        // 2. LÓGICA DE ANIMACIÓN (Balanceo de Spiderman con función seno)
        
        // El movimiento horizontal (eje X) usa la función seno para ir de un lado a otro.
        // * 0.1: Velocidad del balanceo
        // * 20: Amplitud (qué tan lejos se balancea del centro 50)
        double amplitud = 20.0;
        double velocidad = 0.1;

        int offsetX = static_cast<int>(std::sin(frames * velocidad) * amplitud);
        gestor.dibujos[indice_spiderman].x = 50 + offsetX;

        // El movimiento vertical (eje Y) usa el valor absoluto del seno (abs)
        // para que Spiderman solo "baje" (se hunda) al pasar por el centro.
        // * 3: Amplitud vertical (qué tan bajo cae)
        int offsetY = static_cast<int>(std::abs(std::sin(frames * velocidad) * 3));
        gestor.dibujos[indice_spiderman].y = 10 + offsetY; 

        // 3. DIBUJAR TODO
        cielo.Dibujar(screen); // Dibuja el cielo (fondo)
        gestor.DibujarTodos(screen); // Dibuja edificios y Spiderman

        // 4. MOSTRAR EN PANTALLA
        std::cout << screen.ToString();
        std::cout << screen.ResetPosition(); // Regresa el cursor al inicio
        std::cout << std::flush;

        // 5. Esperar el tiempo del frame y aumentar el contador
        frames++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 100ms = 10 FPS
    }
    return 0;
}