## JS

```json
[
  {
    "view": 3.2549999887123704
  },
  {
    "msgCtor": ["GotHomeMsg", "GotTimeZone"],
    "tUpdate": 0.5949999904260039,
    "tStepper": 0.1550000160932541,
    "tEffects": 0.06999995093792677
  },
  {
    "view": 0.5600000731647015
  },
  {
    "msgCtor": ["GotHomeMsg", "PassedSlowLoadThreshold"],
    "tUpdate": 0.2649999223649502,
    "tStepper": 0.06500002928078175,
    "tEffects": 0.05500006955116987
  },
  {
    "view": 0.7050000131130219
  },
  {
    "msgCtor": ["GotHomeMsg", "CompletedFeedLoad"],
    "tUpdate": 0.03999995533376932,
    "tStepper": 0.04499999340623617,
    "tEffects": 0.0350000336766243
  },
  {
    "view": 12.665000045672059
  },
  {
    "msgCtor": ["GotHomeMsg", "CompletedTagsLoad"],
    "tUpdate": 0.07499998901039362,
    "tStepper": 0.11999998241662979,
    "tEffects": 0.0950000248849392
  },
  {
    "view": 14.764999970793724
  }
]
```

## Wasm

But it's broken so most of this is not comparable at all
UI says both API calls failed. Must be something in the Json decoding.

```json
[
  {
    "view": 7.660000002942979
  },
  {
    "msgCtor": ["GotHomeMsg", "GotTimeZone"],
    "tUpdate": 0.2500000409781933,
    "tStepper": 0.23999996483325958,
    "tEffects": 0.4249999765306711
  },
  {
    "view": 5.1499999826774
  },
  {
    // command in Page.Home.init
    "msgCtor": ["GotHomeMsg", "PassedSlowLoadThreshold"],
    "tUpdate": 0.17500005196779966,
    "tStepper": 0.10499998461455107,
    "tEffects": 0.3649999853223562
  },
  {
    "view": 4.175000009126961
  },
  {
    // command in Page.Profile.fetchFeed
    // Json decoder is Article.Feed.decoder
    "msgCtor": ["GotHomeMsg", "CompletedFeedLoad"],
    "tUpdate": 2.3250000085681677,
    "tStepper": 0.0849999487400055,
    "tEffects": 0.30499999411404133
  },
  {
    "view": 1.5450000064447522
  },
  {
    // Page.Home.init
    // Json decoder is Article.Tag.list
    "msgCtor": ["GotHomeMsg", "CompletedTagsLoad"],
    "tUpdate": 0.08000002708286047,
    "tStepper": 1.0400000028312206,
    "tEffects": 0.1550000160932541
  },
  {
    "view": 1.550000044517219
  }
]
```
