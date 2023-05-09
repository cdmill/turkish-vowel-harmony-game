# Turkish Vowel Harmony

## Vowel Inventory

| Front Unrounded | Front Rounded | Back Unrounded | Back Rounded |
| --------------- | ------------- | -------------- | ------------ |
| i, e            | ü, ö          | a, ı           | o, u         | 

Front vowels are produced with the tongue positioned in the front of the mouth, while back vowels are produced with the tongue positioned in the back of
the mouth.
Rounded vowels are produced with the lips rounded, while unrounded vowels are produced with no rounding of the lips.

## Vowel Harmony

In the examples below a `+` indicates the start of a suffix and a `_` indicates a slot for a vowel.

Words in Turkish are built using suffixes. An example is given below.

```
çocuk 'child'
çocuk+lar -> çocuklar 'children'
çocuk+lar+ım -> çocuklarım 'my children'
çocuk+lar+ım+dan -> çocuklarımdan 'from my children'
```

When a suffix is added to a word (stem), the vowels in the suffix must assimilate to the preceding vowels. For example, if the stem contains
a front vowel `ü`, then the subsequent suffixes must also contain front vowels (in the example below this is realized as `e` and `i`).

```
gün 'day'
gün+l_r -> günler 'days'
gün+l_r+_m -> günlerim 'my days'
gün+l_r+_m+d_n -> günlerimden 'from my days'
```

If a stem ends in a vowel, then the vowel in the suffix attached to it is omitted.

```
anne 'mother'
anne+_m -> annem 'my mother'
```

---

### Types of Vowel Harmony

Some suffixes assimilate only in **backness**, ie front vowels or back vowels. Suffixes that only assimilate in backness have alternations between `e`
and `a`.

An example of this is the plural suffix `+l_r`. In the first example below, notice how the vowels in the stem `çocuk` are back and *rounded*. Here, the
vowel in the plural suffix `+l_r` surfaces as `a`, which is harmonized in backness but is *unrounded*. 

In the second example below, notice how the vowel in the stem  `gün` is front and *rounded*. Here, the vowel in the plural suffix `+l_r` surfaces as
`e`, which is harmonized for backness (both vowels are front vowels) but is *unrounded*. 

```
çocuk+l_r -> çocuklar 'children'
gün+l_r -> günler 'days'
```

---
Other suffixes assimilate in both **backness** and **rounding**. Suffixes that assimilate in backness *and* rounding have alternations between `i,ı,ü,u`. 

An example of this is the first-person singular suffix `+_m`. In the first example below, notice how the vowel in the stem
`gün` is a front, rounded vowel. The vowel in the first-person singular suffix `+_m` then assimilates in both backness and rounding and surfaces as
`ü`. 

In the second example, the vowel in the first-person singular suffix `+_m` assimilates to the preceding vowel in the stem `kol` in backness
and rounding, and surfaces as `u`. 

```
gün+_m -> günüm 'my day'
kol+_m -> kolum 'my arm'
```
