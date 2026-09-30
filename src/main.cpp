
#include <chrono>
#include <string>
#include <functional>
#include <format>

#include <SFML/Graphics.hpp>

#include <YOBA/Core.hpp>
#include <YOBA/Rendering.hpp>
#include <YOBA/UI.hpp>
#include <YOBA/Resources/Fonts/Unscii16Font.hpp>

int main() {
	using namespace YOBA;

	// -------------------------------- SFML window --------------------------------

	// Creating window that will simulate 240x320 display, which is widely used among Arduino kids
	// To avoid eye bleeding, let's double the rendering scale
	constexpr static Size screenResolution { 240, 320 };
	constexpr static float renderingScale = 2;

	sf::RenderWindow SFWindow {
		sf::VideoMode({
			static_cast<uint32_t>(static_cast<float>(screenResolution.getWidth()) * renderingScale),
			static_cast<uint32_t>(static_cast<float>(screenResolution.getHeight()) * renderingScale)
		}),
		"YOBA | Desktop demo",
		sf::Style::None | sf::Style::Titlebar | sf::Style::Close,
		sf::State::Windowed
	};

	// -------------------------------- YOBA renderer & rendering target --------------------------------

	// Creating rendering target that encapsulates SFML sprite - it will be used by YOBA for flushing pixel data
	// The sprite itself can be rendered later via window.draw()
	SFMLRenderingTarget renderingTarget {};
	renderingTarget.setup(screenResolution);
	renderingTarget.setRenderingScale(renderingScale);

	// Creating straightforward renderer that doesn't care about CPU/RAM bearing (like RGB565 or Indexed does)
	SFMLRenderer renderer {};
	renderer.setTarget(&renderingTarget);

	// -------------------------------- Colors & fonts  --------------------------------

	// Defining some colors to style controls
	ARGBColor blackColor { 0xFF000000 };
	ARGBColor whiteColor { 0xFFFFFFFF };
	ARGBColor yellowColor { 0xFFffd200 };
	ARGBColor darkYellowColor { 0xFF997e53 };

	// Using one of the sexiest pixelated fonts ever created
	Unscii16Font font {};

	// -------------------------------- UI components  --------------------------------

	// Creating an application that will store
	Application application {};
	application.setRenderer(&renderer);
	application.setBackgroundColor(&blackColor);

	// Creating vertical stack layout to orient text view & button
	StackLayout rows {};
	rows.setGap(10);
	rows.setAlignment(Alignment::center);
	application += &rows;

	// Creating text view to display button dick size
	TextView textView {};
	textView.setFont(&font);
	textView.setTextColor(&whiteColor);
	textView.setTextAlignment(Alignment::center);
	rows += &textView;

	// Storing dick size somewhere
	size_t dickSize = 0;

	const auto updateTextView = [&] {
		textView.setText(std::format("Dick size: {} cm", dickSize));
	};

	updateTextView();

	// Creating button to increment dick size on click
	TextButton button {};
	button.setSize({ 180, 32});
	button.setCornerRadius(4);
	button.setDefaultBackgroundColor(&yellowColor);
	button.setDefaultTextColor(&blackColor);
	button.setActiveBackgroundColor(&darkYellowColor);
	button.setActiveTextColor(&blackColor);
	button.setFont(&font);
	button.setText("Increase");

	button.setOnClick([&] {
		dickSize++;
		updateTextView();
	});

	rows += &button;

	// -------------------------------- Main loop with SFML event handling --------------------------------

	while (SFWindow.isOpen()) {
		// Polling SFML events
		while (const auto event = SFWindow.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				SFWindow.close();
			}
			else {
				// Translating SFML events to YOBA events if they have similar nature (pointer, drag, scroll, etc.)
				SFMLEvents::handleMouse(event, &application, renderingTarget.getRenderingScale());
			}
		}

		// Handling enqueued events, polling HIDs, playing animations, calling onTick(), etc.
		application.tick();
		// Computing size of UI elements & arranging them in the screen space
		application.updateLayout();
		// Rendering UI on assigned rendering target (SFML window in this case)
		application.render();

		// Rendering FPS counter on SFML window
		SFWindow.draw(renderingTarget.getSprite());
		// Finally, displaying all buffered changes in SFML window
		SFWindow.display();
	}
}
