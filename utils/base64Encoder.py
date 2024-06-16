import base64

with open("wrench.png", "rb") as image_file:
    encoded_string = base64.b64encode(image_file.read()).decode("utf-8")
    print(encoded_string)
    # Output :
    """
    iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAM5JREFUOI1jZCAS8Cuo/IexPz64w0isPrhmz3kb/nvO2/Af2SCSNJuXtWDVzEKSaWgGMzAwMDCRokkjLIGBX0HlP7+Cyn+NsAQGBgYGBryBAbOFS1ySQc7eHUP+xqoFuF3Ar6Dyn0tckkEjLIHh28vnDI8O7sTQ/PHBHUasLoBpNkgvZnh34wpcAzKARSVGIGLTDLMdW/wzEqP528vnOBMPIyWa4QaQq5mBgYGBEaYZOZqI1czAgJSQTnbVkKwZxQUwQIpmBgakMIAJkJpVAdF9g4siKN0uAAAAAElFTkSuQmCC
    """

with open("macrobox.png", "rb") as image_file:
    encoded_string = base64.b64encode(image_file.read()).decode("utf-8")
    print(encoded_string)
    # Output :
    """
    iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAO5JREFUOI1j5FdQ+c9AAWBhYGBgaDATJ1nj9cffGFY+/8zAxMDAwODlpQ2XKFh5hKFg5REMDbjEmXDZgKwYm0YYYMHnTHwaYYCRX0HlvwMPTofgBQe+/IN4oavEgcFBS5TBQUuUYcPlmwwbLt/Ey3bQEmUQ5+Um7IUJ4TYEXYFhAC5NuMRZGBgYGNTiZzIwMKQTtA0FbLvKwPD8M8SAdCsjhvvvuYnSx878j4GBgYHh518mhAs0ZbkYGBi+kuQAmIUoYQAxCAKuP/6GVeOzT5wofJITgBTfdwYpvu9wPooLcNmKy3YGBmhKJNUVyAAA    +c1KTAbWe2MAAAAASUVORK5CYII=
    """
