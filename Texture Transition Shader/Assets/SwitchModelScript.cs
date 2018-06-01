using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SwitchModelScript : MonoBehaviour {

    public GameObject[] shipCenters;

    int currentShip = -1;

	// Use this for initialization
	void Start ()
    {
        SwitchShip();
	}
	
	// Update is called once per frame
	void Update ()
    {
		
	}

    public void SwitchShip()
    {
        if (currentShip < shipCenters.Length - 1)
        {
            currentShip++;
        }
        else
        {
            currentShip = 0;
        }

        foreach (var centre in shipCenters)
        {
            centre.transform.position = new Vector3(0, 100, 0);
        }
        shipCenters[currentShip].transform.position = new Vector3(0, 0, 0);
    }
}
