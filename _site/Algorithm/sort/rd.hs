main = do  
        contents <- readFile "/tmp/a.txt"
        print . map readInt . words $ contents
-- alternately, main = print . map readInt . words =<< readFile "test.txt"

readInt :: String -> Int
readInt = read
