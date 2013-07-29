import Data.List 
import Data.Ord
-- selection sort

selection_sort' :: Ord a=>[a]->[a]->Int->([a],Int)
selection_sort' sorted [] i = (sorted,i)
selection_sort' sorted unsorted i 
		  | length(unsorted)==0 =(sorted,i)
		  | otherwise 		= selection_sort' (minx:sorted) ( delete minx unsorted ) (i+1)
		where
		minx=(maximum unsorted)
selection_sort :: Ord a => [a]->([a],Int)
selection_sort s = selection_sort' [] s 0


