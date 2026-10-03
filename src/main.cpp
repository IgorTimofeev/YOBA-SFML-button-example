
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

	// -------------------------------- YOBA renderer & rendering target --------------------------------

	// Creating rendering target that will simulate 240x320 display, which is widely used among Arduino kids
	// Target itself encapsulates SFML sprite - it be rendered later via SFMLWindow.draw()
	SFMLRenderingTarget renderingTarget {};
	renderingTarget.setup({ 240, 320 });
	renderingTarget.setRenderingScale(2.0f);

	// Creating straightforward renderer that doesn't care about CPU/RAM bearing (like RGB565 or Indexed does)
	SFMLRenderer renderer {};
	renderer.setup();
	renderer.setTarget(&renderingTarget);

	// -------------------------------- Colors & fonts  --------------------------------

	// Defining some colors to style controls
	ARGBColor blackColor { 0xFF000000 };
	ARGBColor whiteColor { 0xFFFFFFFF };
	ARGBColor yellowColor { 0xFFFFD200 };
	ARGBColor darkYellowColor { 0xFF997E53 };

	// Using one of the sexiest pixelated fonts ever created
	Unscii16Font font {};

	// -------------------------------- UI components  --------------------------------

	// Creating main application that will take care of child elements
	Application application {};
	application.setRenderer(&renderer);
	application.setBackgroundColor(&blackColor);

	// Creating vertical stack layout for text view & button
	StackLayout rows {};
	rows.setGap(10);
	rows.setAlignment(Alignment::center);
	application += &rows;

	// Creating text view to display dick size
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

	// Creating button that will increment dick size on click
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

	// -------------------------------- SFML window & main loop --------------------------------

	// Creating window that will be used for rendering pixel data
	sf::RenderWindow SFWindow {
		sf::VideoMode({
			static_cast<uint32_t>(static_cast<float>(renderingTarget.getSize().getWidth()) * renderingTarget.getRenderingScale()),
			static_cast<uint32_t>(static_cast<float>(renderingTarget.getSize().getHeight()) * renderingTarget.getRenderingScale())
		}),
		"YOBA | Desktop demo",
		sf::Style::None | sf::Style::Titlebar | sf::Style::Close,
		sf::State::Windowed
	};

	while (SFWindow.isOpen()) {
		// Polling SFML events
		while (const auto event = SFWindow.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				SFWindow.close();
			}
			else {
				// Translating SFML events into YOBA events if they have similar nature (pointer, drag, scroll, etc.)
				SFMLEvents::translate(event, &application, renderingTarget.getRenderingScale());
			}
		}

		// Handling enqueued events, polling HIDs, playing animations, calling onTick(), etc.
		application.tick();
		// Computing size of UI elements & arranging them in the screen space
		application.updateLayout();
		// Rendering UI on assigned rendering target (SFML sprite in this case)
		application.render();

		// Rendering SFML sprite on window & displaying changes
		SFWindow.draw(renderingTarget.getSprite());
		SFWindow.display();
	}
}
