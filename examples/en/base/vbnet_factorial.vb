' VB.NET Syntax POC - Factorial Calculator
' Multi-Syntax Showcase: %100 VB.NET Compliance
' #dil:VBNet
' #sözdizimi:vbnet

' Strict Rules:
' - Dim ZORUNLU (mandatory variable declaration)
' - As ZORUNLU (mandatory type annotation)
' - Then ZORUNLU (mandatory in If statements)
' - End+Keyword ZORUNLU (End Function, End If, End While)
' - Case INSENSITIVE (Function = function = FUNCTION)

Function Factorial(n As Integer) As Integer
    Dim result As Integer = 1
    Dim i As Integer = 1
    
    While i <= n
        result = result * i
        i = i + 1
    End While
    
    Return result
End Function

Function IsPrime(num As Integer) As Boolean
    Dim i As Integer = 2
    Dim found As Boolean = False
    
    If num < 2 Then
        Return False
    End If
    
    While i * i <= num
        Dim remainder As Integer = num
        ' Modulo calculation (MeLP doesn't have % yet)
        While remainder >= i
            remainder = remainder - i
        End While
        
        If remainder = 0 Then
            found = True
        End If
        
        i = i + 1
    End While
    
    If found Then
        Return False
    Else
        Return True
    End If
End Function

Function Main() As Integer
    Dim x As Integer = 5
    Dim fact As Integer
    Dim prime As Boolean
    
    ' Test factorial
    fact = Factorial(x)
    Print("Factorial of 5: ")
    Print(fact)
    Print("\n")
    
    ' Test prime checking
    Dim testNum As Integer = 7
    prime = IsPrime(testNum)
    
    If prime Then
        Print("7 is prime")
    Else
        Print("7 is not prime")
    End If
    
    Return 0
End Function
