#pragma once
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <functional>
#include <cstddef>

namespace sf
{
	enum class TextCircleHighlightMode {NONE, RED, GREEN, BLUE, WHITE };

	/// <summary>
	/// A Simple Circle with text centered in it
	/// </summary>
	class TextCircle : public Drawable
	{
	protected:
		/// <summary>
		/// The SFML text object 
		/// </summary>
		sf::Text mText;

		/// <summary>
		/// The SFML circle object
		/// </summary>
		sf::CircleShape mCircle;

		/// <summary>
		/// The bounding area for the text
		/// </summary>
		sf::FloatRect mTextRect;

		/// <summary>
		/// What kind of highlight, if any to use
		/// </summary>
		TextCircleHighlightMode mHMode;
	public:
		/// <summary>
		/// Default constructor -- doesn't initialize anything
		/// </summary>
		TextCircle();


		/// <summary>
		/// The Constructor that DOES initialize everything
		/// </summary>
		/// <param name="x">starting x-position of the center of the text/circle</param>
		/// <param name="y">starting y-position of the center of the text/circle</param>
		/// <param name="font_ref">the font to use for the text</param>
		/// <param name="text">the text for the circle</param>
		TextCircle(float x, float y, const Font& font_ref, std::string text);

		/// <summary>
		/// Draws the object to the given render target
		/// </summary>
		/// <param name="target">target to draw to</param>
		/// <param name="states">render state parameters</param>
		virtual void draw(RenderTarget& target, RenderStates states) const override;

		/// <summary>
		/// Setter to set the circle color
		/// </summary>
		/// <param name="c"></param>
		void set_circle_color(const sf::Color& c);


		/// <summary>
		/// Getter to get the current position of the center of the circle and text
		/// </summary>
		/// <returns></returns>
		sf::Vector2f get_position() const;

		/// <summary>
		/// Getter to get the string
		/// </summary>
		/// <returns>the string</returns>
		std::string get_string() const;

		/// <summary>
		/// Gets the circle color
		/// </summary>
		/// <returns></returns>
		sf::Color get_color() const;

		/// <summary>
		/// Sets the current position of the center of the circle and text
		/// </summary>
		/// <param name="p"></param>
		void set_position(const sf::Vector2f& p);

		/// <summary>
		/// Sets the string
		/// </summary>
		/// <param name="s">new string value</param>
		void set_string(const std::string& s);

		/// <summary>
		/// Determines if the given point is within the boundaries of the circle
		/// </summary>
		/// <param name="v">the position of interest</param>
		/// <returns>true if it is within, false if not</returns>
		bool point_inside(sf::Vector2f v) const;


		/// <summary>
		/// Returns true if this TextCircle is equal to the other (by string)
		/// </summary>
		/// <param name="other">the other TextCircle</param>
		/// <returns>true if the strings are the same, false if not</returns>
		bool operator==(const sf::TextCircle& other)
		{
			return mText.getString() == other.mText.getString();
		}


		/// <summary>
		/// Sets the higlight mode
		/// </summary>
		/// <param name="m">new mode</param>
		void set_highlight(const TextCircleHighlightMode& m);
	};
}
