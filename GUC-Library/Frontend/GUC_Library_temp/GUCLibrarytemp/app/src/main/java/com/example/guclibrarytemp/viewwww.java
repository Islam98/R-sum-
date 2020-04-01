package com.example.guclibrarytemp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.InputType;
import android.text.TextWatcher;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridLayout;
import android.widget.Spinner;
import android.widget.TextView;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;

public class viewwww extends AppCompatActivity {

    String HostORip = "";

    GridLayout itemgrid;
    Button ViewItems;
    Button NewItem;
 //   DynamicView dv;
    //   ListView lv;
    private RequestQueue mQueue;

    String username;
    boolean admin;

    //initializing id counters for the texts
    int spcounter = 0;
    int textid = 1000;  //price textview id counter
    int edittextid = 10000;     ///quantity edittext id counter

    //   int pricecount = 5;

    ArrayList<String> ItemName = new ArrayList<String>();
    ArrayList<String> ItemPrice = new ArrayList<String>();
    ArrayList<String> ItemQuantity = new ArrayList<String>();
    ArrayAdapter atnames;

    //Arraylist of items to proceed with it:
    ArrayList<String> BoughtItems;
    //Arraylist of Quantities to proceed with it:
    ArrayList<String> Quantities;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Intent i = getIntent();
        HostORip = i.getStringExtra("HostOrIp");
        username = i.getStringExtra("Username");
        admin = i.getBooleanExtra("isAdmin", true);
        if (!admin && (username != null || !username.equals("")))
        {
            setContentView(R.layout.activity_viewwww);


            itemgrid = findViewById(R.id.ItemGrid);
            //     additem = findViewById(R.id.getItems);
            NewItem = findViewById(R.id.newItem);


            //send request to get the items list
            mQueue = Volley.newRequestQueue(this);
            String url = HostORip + "/Items/getItems";
            JsonArrayRequest jsonreq = new JsonArrayRequest(Request.Method.GET, url, null,
                    new Response.Listener<JSONArray>() {
                        @Override
                        public void onResponse(JSONArray response) {
                            try {
                                for (int i = 0; i < response.length(); i++) {
                                    JSONObject item = response.getJSONObject(i);

                                    String name = item.getString("name");
                                    //      String itemdesc = item.getString("description");
                                    String itemquantity = item.getString("quantity");
                                    String itemprice = String.valueOf(item.getDouble("price"));

                                  /*  tvitems.append("item: " + name + ",\n description: " + itemdesc +
                                            ",\n quantity: " + itemquantity + ".\n \n"); */
                                    ItemName.add(name);
                                    ItemPrice.add(itemprice);
                                    ItemQuantity.add(itemquantity);
                                }
                                atnames = new ArrayAdapter(viewwww.this,
                                        android.R.layout.simple_expandable_list_item_1, ItemName);

                                System.out.println("done");
                            } catch (JSONException e) {
                                e.printStackTrace();
                            }
                        }
                    }, new Response.ErrorListener() {
                @Override
                public void onErrorResponse(VolleyError error) {
                    error.printStackTrace();
                }
            });

            mQueue.add(jsonreq);


    //NewItem button click listener
            NewItem.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {

                    if (ItemName.size() != 0) {
                        if (spcounter < 7)
                            ViewJson();
                        else {
                            ErrorDialog error = new ErrorDialog("     Cannot choose more than 7 items at once");
                            error.show(getSupportFragmentManager(), "Error");
                        }
                    } else {
                        ErrorDialog error = new ErrorDialog("     Wait a while as no items in the list now");
                        error.show(getSupportFragmentManager(), "Error");
                        //                    error.setError();
                    }
                }
            });


    //Continue to checkout button click listener
            ViewItems = findViewById(R.id.ViewItem);
            ViewItems.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    int index = 0;
                    Spinner tempsp = findViewById(index);
                    if (tempsp != null) {
                        proceedtocheckout();
                    }
                }
            });

        }
        else {
            Intent nextintent = new Intent(getApplicationContext(), errorpage.class);
            startActivity(nextintent);
        }
    }

//proceedtocheckout button listener method
    public void proceedtocheckout()
    {
        BoughtItems = new ArrayList<String>();
        Quantities = new ArrayList<String>();
        double totalprice = 0;
        for (int i = 10000; i < edittextid; i++)
        {
            EditText tempedittext = findViewById(i);
            int quantity = Integer.parseInt(tempedittext.getText().toString());
            if(quantity != 0)
            {
                Spinner spname = findViewById(i - 10000);
                String name = spname.getSelectedItem().toString();
                int index = ItemName.indexOf(name);
                int itemsquantity = Integer.parseInt(ItemQuantity.get(index));
                int diffquant = itemsquantity - quantity;
                if(diffquant < 0)
                {
                    int element = (i - 10000 + 1);
                    ErrorDialog error = new ErrorDialog("     Quantity of " + name + " item number "
                            + element + " from bottom of quantity: " + quantity +
                            " is greater than that in the stock," +
                            " NOTE that all elements selected before and after this item are saved");
                    error.show(getSupportFragmentManager(), "Error");
                }
                else
                {
                    itemsquantity -= quantity;
                    ItemQuantity.set(index, itemsquantity+"");

                    BoughtItems.add(name);
                    Quantities.add(quantity+"");

                    TextView tvprice = findViewById(i - 9000);
                    totalprice += Double.parseDouble(tvprice.getText().toString());//send 2 arrays and total price and then place next page
                }
            }
        }
        if(BoughtItems.size() != 0)
        {
            Intent i = new Intent(getApplicationContext(), Checkout.class);
            i.putStringArrayListExtra("itemsname", BoughtItems);
            i.putStringArrayListExtra("itemsquantity", Quantities);
            i.putExtra("Username", username);
            i.putExtra("isAdmin", admin);
            i.putExtra("total price", totalprice);
            i.putExtra("HostOrIp", HostORip);
            startActivity(i);
        }
    }


//Newitem button listener method
    private void ViewJson() {


        //           dv = new DynamicView(context);

        final ViewGroup.LayoutParams lparams = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT);
        final Spinner sp = new Spinner(viewwww.this);
        sp.setLayoutParams(lparams);
        sp.setAdapter(atnames);
        sp.setId(spcounter);
        itemgrid.addView(sp, 3);
        spcounter++;
        System.out.println(spcounter);



        final EditText editText = new EditText(viewwww.this);
        editText.setId(edittextid);
        editText.setMinEms(2);
        editText.setText("0");
        //editText.setTextColor(Color.rgb(0,0,0));
        editText.setInputType(InputType.TYPE_CLASS_NUMBER);
        itemgrid.addView(editText, 4);
        editText.addTextChangedListener(new TextWatcher() {         //text change listener to update price
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {

                if (s == null || s.toString().isEmpty()) {
                } else {
                    int differenceids = edittextid - 10000;
                    int IDindex = 0;
                    //for loop to get the id of the changed edittext box
                    for (int i = 0; i < differenceids; i++) {
                        EditText temp = findViewById(i + 10000);
                        if (temp.getText().hashCode() == s.hashCode()) {
                            IDindex = i + 10000;
                            break;
                        }
                    }
//                   System.out.println(IDindex + " ======================================================================================");
                    if (!(s.toString().isEmpty()))
                        textchanged(s, start, before, count, IDindex);
                }
            }

            @Override
            public void afterTextChanged(Editable s) {
            }
        });
        edittextid++;



//getting the index of item by getting its spinner and get the text on it then
//get the price by the same index from the price list and put it on textview:
        Spinner sp1 = findViewById(spcounter - 1);
        String selectedname = sp1.getSelectedItem().toString();
        int index = ItemName.indexOf(selectedname);
        String price = ItemPrice.get(index);
        TextView tvprice = new TextView(viewwww.this);
        tvprice.setId(textid);
        textid++;
        //         tvprice.setText(price);
        tvprice.setText("0");
        //                  itemgrid.addView(tvprice, pricecount);
        itemgrid.addView(tvprice, 5);
//set price if selected item is changed by the user:
        sp1.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                itemchanged(parent, view, position, id);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
    }


    public void itemchanged(AdapterView<?> parent, View view, int position, long id) {

        int id1 = (int) id;         //id of item changed in the array
        //      String price = ItemPrice.get(id1);
        int id2 = parent.getId();      //id of the spinner of the changed item

        //set quantity to default "zero":
        EditText tempedittext = findViewById(id2 + 10000);
        tempedittext.setText("0");

/*        TextView tvprice = findViewById(id2 + 1000);  //incrementing 1000 to the spinner id as
// the price textview id is initialized with 1000 greater than spinner id to get the price id then change its text value
        tvprice.setText(price);
        */ //this sets price of 0 default quantity with the price of 1 item

        TextView tvprice = findViewById(id2 + 1000);
        tvprice.setText("0");
    }

    public void textchanged(CharSequence s, int start, int before, int count, int id) {
//        System.out.println(s.toString() + "--------------------------------------------------------------------------------------------------------------------------------------");

        //get name of item
        Spinner spItemname = findViewById(id - 10000);
        String name = spItemname.getSelectedItem().toString();

        //get index of name then price with the index we've got:
        int indexofname = ItemName.indexOf(name);
        double priceofitem = Double.parseDouble(ItemPrice.get(indexofname));

        //multiply the price of item with the value entered in quantity:
        int quantityentered = Integer.parseInt(s.toString());
        double newprice = priceofitem * quantityentered;

        //set the price textview that matches the edittext quantity changed with the new price:
        TextView temptvprice = findViewById(id - 9000);
        temptvprice.setText(newprice + "");
    }

}
