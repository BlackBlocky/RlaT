# The algorithm that is used for converting Binary Expressions into an AST

The Alogorithmus is slighly inspired by "The Shunting Yard Algorithm".
The Main Idea is, to convert a Mathematical Expression into many block, or fragments: Putting these into a BinaryTree and looping though these in the reverse order, to create an AST.

The details how the algorithm works will be explained in the following text.

---

## The used array for the expression

Lets take the following Mathematical expression

$$
1+(2*3+4*5)*6
$$

The algorithm will create a blocky for every operator in the expresion. It will start at the mathematically first operator, then it will work its way to last operator in mathematical order.

Every created block will be stored in a Array. The Array will be the size of the Operators in the Expression. The indexes are according to the position of the opeator in the expression (first at index 0, etc...).

For every Opeator, it will create a Fragment, taking the left and right value as its child nodes. In order to make sure that the value wanst already used by another Operator, it will check in the FragmentArray, if the left/right operator already created a Fragment.

Because it could happen, that for example the right Fragment of an operator is already used by a higher fragment, that uses the fragment, we a need solution, to figure out what is the higher ranked fragment. To figure that out, the algorithm creates something like "log". Every time, a opertor uses a another fragment in its fragment, it adds a log in the list, with a pair of the used framgents reference and its own refernce. With that log-list, we can always check if a fragment is already used by another fragment, so we can use the higher ranked fragment.

We finding the higher ranked framgent by iterating through the log-list backwards. If the find the fragments reference in the used side, we use the higher ranked fragment that is paired at the user of the fragment. We repead that until we dont find a use of the last returned fragment.

When these steps are over, the algorithm should have binaryTree with all operators and its left and right value. While the algorithm created this binary tree, it always added a new generated Fragment to a list. So that in the end we have a order when which fragment was created.

Now, we take the last fragment that was created, which conatins the last operator in mathematical order. With that one, we can create a AST Modell, by recursive going though every left and right item of that root fragment. If its a literal, just add it as a literal to the AST, if its a fragment, do the same for that one.

---

## Some examples how the algorithm works

### Example 1

$$
1+(2*3+4*5)*6
$$

The Expression contains 5 operators, so lets create an array of 5 storing everything. Also creating other nessessary arrays.

```cpp
// Fragment Index saver
Fragment* fragmens[5];

// Ordered creation list
vector<Fragment*> sortedFragments;

// Use-Log, left=used, right=user
vector<pair<OpFragment*, OpFragment*>> fragmentUseLog;
```

According to the rules of Math, we starts at the 2*3 expression. We create a fragment and save it at index 1. We also push it into the sortedFragments list.

Next we do 4*5, and we do basically the same.

Now were at 3+4. The algorithm will detected, that framgent index 1 and 3 are already created. So it will link these at Numbers, and wont use the literals 3 and 4.
Because we done that, we need to log the uses into the fragmentUseLog. We push following pairs into the list, using the indexes for this example case (References would be also possible it doesnt matter): <1, 2>, <3, 2>

Next we are at opeation index=4, it will detect that operation index=3 is already a fragment. But after checking the use log, it will see that index=3 was already used by index=2: So it will use the result of index=2 for the left value. The right one will just be the literal 6. It also logs the use of index 2 by index 4 <2, 4>.

Finally, we are at operation index=0. It will use the left literal 1. For the right, it will find the fragment index=1. But checking the use log from last to first, it will detect that it was used by index=2. After the next check it will detect, that index=2 was used by index=4. So it will refer to the result from index=4.

Now finally, we are done creating the fragments. With these fragments, its now easy to create a AST, by using the last generated Fragment.

Down below is a representation of the data containing the algorithm is done (Pseudo Code like):

```cpp
// Fragment Index saver
Fragment* fragmens[5] = {
    Fragment(<Literal>1,   <Index>4  <Op>+)
    Fragment(<Literal>2, <Literal>3  <Op>*)
    Fragment(  <Index>1,   <Index>3  <Op>+)
    Fragment(<Literal>4, <Literal>5  <Op>*)
    Fragment(  <Index>2, <Literal>6  <Op>*)
}

// Ordered creation list
vector<Fragment*> sortedFragments = {
    index=1,
    index=3,
    index=2,
    index=4,
    index=0
}

// Use-Log, left=used, right=user
vector<pair<OpFragment*, OpFragment*>> fragmentUseLog = {
    <1, 2>,
    <3, 2>,
    <2, 4>,
    <4, 0>
}
```
