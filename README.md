####In Development Dragon Ball Online Server using Taiwan Game Client 1.69

- Auth Server:
  - 90% done
  - TODO:
    - check for multi server not working ???

- Char Server:
  - 60% done
  - TODO:
    - Some packet are not handled:
      - UC_CHARACTER_SELECT_REQ
      - UC_CHARACTER_EXIT_REQ
      - UC_CHARACTER_DEL_CANCEL_REQ
      - UC_CONNECT_WAIT_CHECK_REQ
      - UC_CONNECT_WAIT_CANCEL_REQ
      - UC_CHARACTER_RENAME_REQ
      - UC_CASHITEM_HLSHOP_REFRESH_REQ
      - UC_CASHITEM_BUY_REQ
  - Know ISSUES:
    - Can't create more than 5 characters, if it the game don't load them all and disconnect.
    - On deleting a character and sending the notify for the characters list reset and the "CREATE CHARACTER" button disappear.
