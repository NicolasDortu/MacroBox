import base64

with open("wrench.png", "rb") as image_file:
    encoded_string = base64.b64encode(image_file.read()).decode("utf-8")
    print(encoded_string)
    # Output :
    """
    iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAM5JREFUOI1jZCAS8Cuo/IexPz64w0isPrhmz3kb/nvO2/Af2SCSNJuXtWDVzEKSaWgGMzAwMDCRokkjLIGBX0HlP7+Cyn+NsAQGBgYGBryBAbOFS1ySQc7eHUP+xqoFuF3Ar6Dyn0tckkEjLIHh28vnDI8O7sTQ/PHBHUasLoBpNkgvZnh34wpcAzKARSVGIGLTDLMdW/wzEqP528vnOBMPIyWa4QaQq5mBgYGBEaYZOZqI1czAgJSQTnbVkKwZxQUwQIpmBgakMIAJkJpVAdF9g4siKN0uAAAAAElFTkSuQmCC
    """
