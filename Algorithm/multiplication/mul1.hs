--- Karatsuba Multiplication
it import Data.Char (digitToInt)

rmul :: Integer -> Integer -> Integer	---input output type
rmul x y 
     | (base1 == 1) = x*y		---- if any digit is of length 1 ,use default multiplization
     | otherwise=z2*base*base + ((rmul (x1+x0) (y1+y0)) -z2-z0)*base + z0	-- karatsuba multiplication formula
      where
	    n=length . map digitToInt $ show (max x y)	-- no of digits
	    n1=length . map digitToInt $ show (min x y)
            base =10^(div n 2)					
            base1 =10^(div n1 2)
            z2 =rmul x1 y1	-- performing multiplizations
            z0 = rmul x0 y0	-- performing the multiplications
            (x1, x0) = helper x -- get the sub division of number
            (y1, y0) = helper y -- get the sub division of number
            helper n = (div n base, mod n base)




