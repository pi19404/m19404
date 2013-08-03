-- bubble sort
import Data.Char (digitToInt)
import Data.List 
import Data.Ord
import System.IO  
import Control.Monad
import Numeric
import Foreign.Marshal.Safe
import Foreign.Marshal
import System.Random
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
splitN :: Ord a =>[a]->Int->([a],[a],Int)
splitN s i= (l1,l2,(i+1))
	where 
	(l1,l2)=splitAt ( div (length(s)) 2 ) s

--merge sort
-- the function to merge the 2 sorted lists
-- iput is the lists to be sorted and current number of inversions
-- output is merger list + total number of computation required for merge
merge1 :: Ord a =>[a]->[a]->Int->Bool->([a],Int)
merge1 [] xs i _ = (xs,i)
merge1 xs [] i _ = (xs,i)
merge1 (x:xs) (y:ys) i flag 
		|x<y = (x:a1,a2) 
		|otherwise = case flag of
			t|t==False->(y:b1,val2)					
	 		 |otherwise->(y:b1,val1)
		where
		  (a1,a2)=merge1 xs (y:ys) (i) flag
		  (b1,b2)=merge1 (x:xs) ys (i1) flag
		  --c1=x<y = (x:a1,a2)
                  --c2=(y:b1,b2)
		  i1=case flag of 
			t|t==False->(i+1)
			 |otherwise -> i+length(x:xs)
		  val1=b2
		  val2=b2
		  


-- the recrsive for merge sort algorithm
-- input is list ,output is merge sorted list and number of inversions
merge_sort' :: Ord a=>[a]->Int->([a],Int)	
merge_sort' list i  
		| length(list)==1=(list,i)
		| otherwise =(o1,o2)
		where
		(list1,list2,m5)=(splitN list 0)
		(list3,m3)=merge_sort' list1 (0)
		(list4,m4)=merge_sort' list2 (0)
-- call merge with true flag for inversion count		
		(o1,o2)=merge1 list3 list4 (m3+m4) True


-- main function to be called for merge sort algoritm
merge_sort :: Ord a =>[a]->([a],Int)
merge_sort s = merge_sort' s 0

select :: Ord a=>[a]->Int-> ([a],[a])
select (n) i= case i of 
		t| t<=0 -> splitAt (length(n)+t) n
		 |otherwise -> splitAt t n



getWords :: FilePath ->Int-> IO (Int,[Int])
getWords path int1 = do contents <- readFile path
	                return(execute (select (map readInt1 (lines contents)) (int1) )  )
		     where
		     execute (s,s1) =(snd(merge_sort s),s1)


readInt1 :: String -> Int
readInt1 = read

get :: Ord a=>[a]->Int->(a)
get x k = x!!k


-- function to swap the elements of the list
swap1 :: Ord a => [a]->Int->Int->[a]
swap1 xs i j = case i==j of
	      flag|flag==False ->v1 ++ b:a3 ++ v2 ++ a4
	          |otherwise -> xs
	      where
	      (aa,a2)=splitAt (i+1) xs
	      (v1,v2)=splitAt (length(aa)-1) aa
	      (a3,b:a4)=splitAt (j-i-1) a2


-- naive implementation function to partition the list for quick sort algorithm
part_list :: Ord a=>a->[a]->Int->Int->([a],Int,Int)
part_list f [] i j =([],0,0)
part_list f a i j  =case j of
		 j1|(j1==(length(a)-1)) ->(a,i,j)
		   |otherwise ->  (a1,a2,a3)
		where
-- partitioning the list,incrementing i and j based on comparison with
-- the pivot element
		(a1,a2,a3)  | f< (a!!(j+1)) = part_list f a (i) (j+1)
	   		    | otherwise = part_list f (swapo) (i+1) (j+1) 
		swapo=((swap1 a (i+1) (j+1))) 


				       

-- the main method for quick sort algorithm with random generator
qSortIO xs = do g <- getStdGen -- add getStdGen to your imports
                return $ partition_list1 xs (length(xs)) g [] 0

--


-- the main function to be called for paritioning the list for quich sort algorithm
partition_list :: Ord a =>[a]->Int->StdGen->([a])
partition_list []  l g =([]) 
partition_list (f) l g = case length(f)<=2 of
		 	  f1|f1==True -> (a1 ++ pivot:b1)    -- terminating condition if sublists are sorted
			    |otherwise-> (c1 ++ pivot:d1)      -- sort the sublist recursively and then append
		 where
		 -- val1 is pivot,val 2 is list to be partitioned
		 (a1,b1,l1,l2,i)=part_list1 pivot pi f [] [] 0 0 0

		 
		 --- partioning subroutine called in case of sublists
		 --- else list is appended and returned
		 (c1)=partition_list a1 l1 g' 
		 (d1)=partition_list b1 l2 g''
		 (pivotLoc,g')=next g
		 (pivotLoc',g'')=next g'
		 pi=pivotLoc `mod` l
		 pivot=(f)!!pi

test xs= do g <- getStdGen 
	    return $ partition_list1 [200,10,100,7,5] 5 g [] 0

combine :: Ord a=>[a]->[a]->([a])
combine [] [] = []
combine [] acc = acc
combine acc [] = acc
combine [x] acc = x:acc
combine sorted acc = (sorted ++ acc)


-- the quick sort function with accumulator
partition_list1 :: Ord a =>[a]->Int->StdGen->[a]->Int->([a])
partition_list1 []  l g acc la=acc
partition_list1 [x] 1 g acc la=x:acc
-- adding pivot to sorted to get complete sorted list
-- calling quick sort on unsorted list as and input accumulator
-- as current sorted list
partition_list1 f l g acc la= partition_list1 as l1 g' ([pivot] ++ sorted) (l2) 
--(sorted ++ [pivot] ++ as ++ [pivot] ++ bs ++ [pivot]) --
		 where
		 sorted=partition_list1 bs l2 g'' acc la
-- partitioning the unsorted list		  
		 (as,bs,l1,l2,i)=part_list1 pivot pi f [] [] 0 0 0
-- getting instance of random number generator		 
		 (pivotLoc,g')=next g
		 (pivotLoc',g'')=next g'
-- index of pivot		 
		 pi=pivotLoc `mod` l
-- getting pivot element		 
		 pivot=(f)!!pi


-- using accumulator lists to partition a list for quick sort algorithm
-- append elements less than pivot to as
-- append elements greater than pivot to bs
-- i is current index,to skip over pivot element
part_list1 pivot pi [] as bs l1 l2 i= (as,bs,l1,l2,i)
part_list1 pivot pi (y:ys) as bs l1 l2 i=case (i)==pi of
					         flag|flag==True->part_list1 pivot pi ys as bs l1 l2 (i+1)
					    	     |otherwise->f
			 			 where f=case compare y pivot of
		 					 LT->part_list1 pivot pi ys (y:as) bs (l1+1) l2 (i+1)
							 EQ->part_list1 pivot pi ys (y:as) bs (l1+1) l2 (i+1)
		 			                 GT->part_list1 pivot pi ys as (y:bs) l1 (l2+1) (i+1)
		

-- test program to check partition list for quick sort algorithm
run_partition_list i = qSortIO (take i a) 
		     where a=[2,5,3,7,4,2,10,1,2,100,200,2,99,1243,425,123,45,123,125,53,45,32,67,128,857,463,47,98,76,54,12]		


--5 ||3 7 4
--5 3||7 4
--5 3|7| 4
--5 3|4|7 -> 3|4 7|




