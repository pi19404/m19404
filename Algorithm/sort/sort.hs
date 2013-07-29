-- bubble sort
import Data.Char (digitToInt)
import Data.List 
import Data.Ord

-- running the loop till sorting in complete or N iterations :
bubble_sort' :: Ord a => [a] ->Int->([a],Int)
bubble_sort' s i = case (_bsort s) of
	  t | i==(length(s)) || (t==s) ->(t,i)
	    | otherwise ->((bubble_sort' (t) (i+1)))
	 where
	-- x,x2 first 2 elements , xs remaining of the list
	-- compare x with x2 ,if x>x2 replace x with x2 in the list,can again perform comparision with remaining list
	-- if x<x2 then we proceed to compare the remaining elements from x2 to xs
	_bsort(x:x2:xs) | x>x2=x2:(_bsort (x:xs))          -- replace
			| otherwise =x:(_bsort(x2:xs))    -- already sorted	
	_bsort s = s
	

-- returns the sorted list and number of iterations take to perform sorting operation
-- main function for bubble sort
bubble_sort :: Ord a => [a] ->([a],Int)
bubble_sort s = bubble_sort' s 0


-- selection sort
selection_sort' :: Ord a=>[a]->[a]->Int->([a],Int)
-- required with no elements in unsorted list
selection_sort' sorted [] i = (sorted,i)		
selection_sort' sorted unsorted i 			
-- when no elements in unsorted list ,another way
		  | length(unsorted)==0 =(sorted,i)	
		  | otherwise = selection_sort' (minx:sorted) ( delete minx unsorted ) (i+1) 
-- elements in unsorted list,find the maximum 
-- remove from unsorted list
-- add to head of the sorted list
		where
		minx=(maximum unsorted)	-- find the maximum in unsorted list
-- main function for selection sort
selection_sort :: Ord a => [a]->([a],Int)
selection_sort s = selection_sort' [] s 0



-- insertion sort

-- function place element x is right position in list (y:ys)
_isort :: Ord a=>a->[a]->[a]
_isort x []= [x]
--- if ordered position found 
_isort x (y:ys) | x<y = x:y:ys 
 -- finding ordered position after y in ys
		| otherwise = y:(_isort x ys)
	

--return sorted list and number of iterations
insertion_sort' :: Ord a=>[a]->[a]->Int->([a],Int)	
-- no elements in unsorted list
insertion_sort' sorted [] i = (sorted,i)	
-- recursive call
insertion_sort' sorted (x:xs) i = insertion_sort' ( _isort x sorted ) xs (i+1)

-- main function for insertion sort			
insertion_sort :: Ord a => [a]->([a],Int)
insertion_sort s = insertion_sort' [] s 0



--split 
-- split an array of size N into two arrays of size N/2
-- the computation of total length of the list is required
split :: Ord a =>[a]->Int->([a],[a],Int)
split s i= (l1,l2,(i+1))
	where 
	(l1,l2)=splitAt ( div (length(s)) 2 ) s

--merge
-- the function to merge the 2 sorted lists
-- iput is the lists to be sorted and current number of computation
-- output is merger list + total number of computation required for merge
merge1 :: Ord a =>[a]->[a]->Int->([a],Int)
merge1 [] xs i = (xs,i+1)
merge1 xs [] i = (xs,i+1)
merge1 (x:xs) (y:ys) i
		|x<y = (x:a1,a2)
		|otherwise = (y:b1,b2)
		where
		  (a1,a2)=merge1 xs (y:ys) (i+1)
		  (b1,b2)=merge1 (x:xs) ys (i+1)


-- the recrsive for merge sort algorithm
-- input is list ,output is merge sorted list and number of computation
merge_sort' :: Ord a=>[a]->Int->([a],Int)	
merge_sort' list i  
		| length(list)==1=(list,i)
		| otherwise =(o1,o2)
		where
		(list1,list2,m5)=(split list 0)
		(list3,m3)=merge_sort' list1 0
		(list4,m4)=merge_sort' list2 0
		(o1,o2)=merge1 list3 list4 (m3+m4+m5)


-- main function to be called for merge sort algoritm
merge_sort :: Ord a =>[a]->([a],Int)
merge_sort s = merge_sort' s 0




